import { Grid, Tile, type Vec2 } from './grid'

const DIRECTIONS: Vec2[] = [
  { x: 1, y: 0 },
  { x: -1, y: 0 },
  { x: 0, y: 1 },
  { x: 0, y: -1 },
]

const add = (a: Vec2, b: Vec2): Vec2 => ({ x: a.x + b.x, y: a.y + b.y })

// Number of free cells reachable from start
function floodCount(grid: Grid, start: Vec2) {
  const visited = new Uint8Array(grid.cells.length)
  const stack = [start]
  let count = 0

  while (stack.length) {
    const pos = stack.pop()!
    const i = pos.y * grid.width + pos.x
    if (!grid.isFree(pos) || visited[i]) continue
    visited[i] = 1
    count++
    for (const dir of DIRECTIONS) stack.push(add(pos, dir))
  }
  return count
}

// Number of free cells cut off from the rest of the board, Infinity if the head is cornered
function dangerLevel(grid: Grid, head: Vec2) {
  const neighbours = DIRECTIONS.filter((dir) => grid.isFree(add(head, dir))).length
  if (neighbours <= 1) return Infinity

  let expected = 0
  let start: Vec2 | null = null
  for (let y = 0; y < grid.height; y++) {
    for (let x = 0; x < grid.width; x++) {
      if (grid.get({ x, y }) !== Tile.Snake) {
        expected++
        start = { x, y }
      }
    }
  }
  return expected - (start ? floodCount(grid, start) : 0)
}

// Length of the shortest path to the food (both ends included), 0 if unreachable
function pathLength(grid: Grid, start: Vec2) {
  const visited = new Uint8Array(grid.cells.length)
  let queue = [start]

  for (let length = 1; queue.length; length++) {
    const next: Vec2[] = []
    for (const pos of queue) {
      if (grid.get(pos) === Tile.Food) return length
      for (const dir of DIRECTIONS) {
        const n = add(pos, dir)
        const i = n.y * grid.width + n.x
        if (grid.isFree(n) && !visited[i]) {
          visited[i] = 1
          next.push(n)
        }
      }
    }
    queue = next
  }
  return 0
}

// Prefer the safest move, then the one closest to the food
export function botDirection(grid: Grid, head: Vec2): Vec2 {
  let next = head
  let bestDanger = Infinity
  let bestDist = Infinity

  for (const dir of DIRECTIONS) {
    const pos = add(head, dir)
    if (!grid.isFree(pos)) continue

    const copy = grid.clone()
    copy.set(pos, Tile.Snake)

    const danger = dangerLevel(copy, pos)
    const isFood = grid.get(pos) === Tile.Food

    if (isFood && !danger) {
      next = pos
      break
    }

    const dist = isFood ? 1 : pathLength(copy, pos) || Infinity

    if (danger < bestDanger || (danger === bestDanger && dist <= bestDist)) {
      next = pos
      bestDanger = danger
      bestDist = dist
    }
  }

  return { x: next.x - head.x, y: next.y - head.y }
}
