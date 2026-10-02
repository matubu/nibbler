import { Grid, type Vec2 } from './grid'

export type BotSnake = { parts: Vec2[]; isDead: boolean; direction: Vec2 }

const DIRECTIONS: Vec2[] = [
  { x: 0, y: -1 },
  { x: 1, y: 0 },
  { x: 0, y: 1 },
  { x: -1, y: 0 },
]

const add = (a: Vec2, b: Vec2): Vec2 => ({ x: a.x + b.x, y: a.y + b.y })
const manhattan = (a: Vec2, b: Vec2) => Math.abs(a.x - b.x) + Math.abs(a.y - b.y)

// Number of moves before each cell is free: snakes leave a cell when their tail passes,
// dead snakes never do
function freeTimes(grid: Grid, snakes: BotSnake[]) {
  const times = new Float64Array(grid.cells.length)
  for (const snake of snakes) {
    snake.parts.forEach((part, k) => {
      if (grid.isOutOfBounds(part)) return
      times[part.y * grid.width + part.x] = snake.isDead ? Infinity : snake.parts.length - k
    })
  }
  return times
}

// Moves needed to reach each cell (-1 if never), entering cells only once they are free.
// `parents` leads back to the start, `count` is the number of reachable cells.
function explore(grid: Grid, times: Float64Array, start: Vec2, startDistance: number) {
  const distances = new Int32Array(grid.cells.length).fill(-1)
  const parents = new Int32Array(grid.cells.length).fill(-1)
  distances[start.y * grid.width + start.x] = startDistance
  let queue = [start]
  let count = 0

  for (let distance = startDistance; queue.length; distance++) {
    const next: Vec2[] = []
    for (const pos of queue) {
      count++
      for (const dir of DIRECTIONS) {
        const n = add(pos, dir)
        if (grid.isOutOfBounds(n)) continue
        const i = n.y * grid.width + n.x
        if (distances[i] !== -1 || times[i] > distance) continue
        distances[i] = distance + 1
        parents[i] = pos.y * grid.width + pos.x
        next.push(n)
      }
    }
    queue = next
  }
  return { distances, parents, count }
}

// Whether the snake can still reach its tail once it has eaten at the end of this path,
// meaning it can never get trapped by its own body
function canEatSafely(grid: Grid, times: Float64Array, self: BotSnake, path: Vec2[]) {
  const virtualTimes = times.map((time) => Math.max(0, time - path.length))
  for (const part of self.parts) {
    if (!grid.isOutOfBounds(part)) virtualTimes[part.y * grid.width + part.x] = 0
  }
  const body = [...path].reverse().concat(self.parts).slice(0, self.parts.length + 1)
  body.forEach((part, k) => {
    if (!grid.isOutOfBounds(part)) virtualTimes[part.y * grid.width + part.x] = body.length - k
  })
  const tail = body[body.length - 1]
  return explore(grid, virtualTimes, body[0], 0).distances[tail.y * grid.width + tail.x] !== -1
}

type Move = {
  dir: Vec2
  // Cells reachable from there
  space: number
  // Moves to the snake's own tail, -1 if it can't get back to it
  tailDistance: number
  // Another snake's head could take the same cell next tick
  contested: boolean
}

function isBetter(a: Move, b: Move) {
  const aSafe = a.tailDistance !== -1
  const bSafe = b.tailDistance !== -1
  if (aSafe !== bSafe) return aSafe
  if (!aSafe) return a.space > b.space // trapped anyway: survive as long as possible
  if (a.contested !== b.contested) return !a.contested
  // Take the long way around to the tail: it buys time and keeps the body tight
  return a.tailDistance > b.tailDistance
}

// Go for the food when no other snake is closer and eating can't trap the snake,
// otherwise stall by following the tail
export function botDirection(grid: Grid, snakes: BotSnake[], self: BotSnake, food: Vec2): Vec2 {
  const head = self.parts[0]
  const tail = self.parts[self.parts.length - 1]
  const others = snakes.filter((snake) => snake !== self && !snake.isDead)
  const isContested = (pos: Vec2) => others.some((snake) => manhattan(snake.parts[0], pos) === 1)
  const times = freeTimes(grid, snakes)

  const chasing = others.every((snake) => manhattan(snake.parts[0], food) >= manhattan(head, food))
  if (chasing) {
    const { parents } = explore(grid, times, head, 0)
    const path: Vec2[] = []
    for (let i = food.y * grid.width + food.x; parents[i] !== -1; i = parents[i]) {
      path.unshift({ x: i % grid.width, y: Math.floor(i / grid.width) })
    }
    if (path.length && !isContested(path[0]) && canEatSafely(grid, times, self, path)) {
      return { x: path[0].x - head.x, y: path[0].y - head.y }
    }
  }

  let best: Move | null = null
  for (const dir of DIRECTIONS) {
    const pos = add(head, dir)
    if (!grid.isFree(pos)) continue

    const { distances, count } = explore(grid, times, pos, 1)
    const move: Move = {
      dir,
      space: count,
      tailDistance: grid.isOutOfBounds(tail) ? -1 : distances[tail.y * grid.width + tail.x],
      contested: isContested(pos),
    }
    if (!best || isBetter(move, best)) best = move
  }

  return best?.dir ?? self.direction
}
