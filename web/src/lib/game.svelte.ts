import { play, setAudioEnabled, stop } from './audio'
import { botDirection } from './bot'
import type { Config, Control } from './config'
import { Grid, Tile, tilesIn, type Vec2 } from './grid'

export type SnakePart = Vec2 & { isEating: boolean }

// Holding R would reset on every key repeat
const RESET_COOLDOWN_MS = 100

export class Snake {
  parts = $state<SnakePart[]>([])
  isDead = $state(false)
  score = $state(0)
  direction: Vec2 = { x: 0, y: -1 }
  control: Control

  // x and y from the center of the snake
  constructor(x: number, y: number, control: Control) {
    this.parts = [-2, -1, 0, 1].map((dy) => ({ x, y: y + dy, isEating: false }))
    this.control = control
  }

  // Returns true if the snake has eaten
  update(grid: Grid, snakes: Snake[], food: Vec2) {
    if (this.control === 'bot') {
      this.direction = botDirection(grid, snakes, this, food)
    }

    const pos = { x: this.parts[0].x + this.direction.x, y: this.parts[0].y + this.direction.y }

    if (!grid.isFree(pos)) {
      this.isDead = true
      return false
    }

    const isEating = grid.get(pos) === Tile.Food
    this.parts.unshift({ ...pos, isEating })

    if (isEating) {
      this.score++
    } else {
      this.parts.pop()
    }
    return isEating
  }
}

export class Game {
  readonly config: Config

  width = $state(0)
  height = $state(0)
  snakes = $state<Snake[]>([])
  food = $state<Vec2>({ x: 0, y: 0 })
  speed = $state(0)
  gameOver = $state(false)

  #nextReset = 0

  constructor(config: Config, viewportWidth: number, viewportHeight: number) {
    this.config = config
    this.speed = config.speed
    setAudioEnabled(config.music)
    this.fit(viewportWidth, viewportHeight)
    this.reset()
  }

  // Resize the grid to fill the viewport.
  // The game goes on: snakes left outside die on their next move.
  fit(viewportWidth: number, viewportHeight: number) {
    this.width = tilesIn(viewportWidth)
    this.height = tilesIn(viewportHeight)
    if (this.food.x >= this.width || this.food.y >= this.height) {
      this.spawnFood()
    }
  }

  get scoreText() {
    return 'Score: ' + this.snakes.map((snake) => snake.score).join(' | ')
  }

  reset() {
    const now = performance.now()
    if (now < this.#nextReset) return
    this.#nextReset = now + RESET_COOLDOWN_MS

    // Snakes spread evenly in rows, with at least one free column between them
    const controls = this.config.players
    const perRow = Math.min(controls.length, Math.floor(this.width / 2))
    const rows = Math.ceil(controls.length / perRow)
    const spread = (i: number, count: number, length: number) => Math.floor((length * (i + 1)) / (count + 1))

    this.gameOver = false
    this.snakes = controls.map((control, i) => {
      const row = Math.floor(i / perRow)
      const inRow = Math.min(perRow, controls.length - row * perRow)
      return new Snake(spread(i % perRow, inRow, this.width), spread(row, rows, this.height), control)
    })
    this.spawnFood()
    play('music', true)
  }

  grid() {
    const grid = new Grid(this.width, this.height)
    grid.set(this.food, Tile.Food)
    for (const snake of this.snakes) {
      for (const part of snake.parts) {
        if (!grid.isOutOfBounds(part)) grid.set(part, Tile.Snake)
      }
    }
    return grid
  }

  spawnFood() {
    const grid = this.grid()
    while (true) {
      const food = {
        x: Math.floor(Math.random() * this.width),
        y: Math.floor(Math.random() * this.height),
      }
      if (grid.get(food) !== Tile.Snake) {
        this.food = food
        return
      }
    }
  }

  update() {
    if (this.gameOver) return

    let allDead = true

    for (const snake of this.snakes) {
      if (snake.isDead) continue

      if (snake.update(this.grid(), this.snakes, this.food)) {
        this.spawnFood()
        play('eat')
      }

      if (snake.isDead) {
        play('die')
      } else {
        allDead = false
      }
    }

    if (allDead) {
      this.gameOver = true
      stop('music')
    }
  }

  // Turn every snake driven by these keys, unless it would go back into its neck
  changeDirection(control: Control, direction: Vec2) {
    for (const snake of this.snakes) {
      if (snake.control !== control) continue
      const [head, neck] = snake.parts
      if (head.x + direction.x === neck.x && head.y + direction.y === neck.y) continue
      snake.direction = direction
    }
  }

  speedUp() {
    this.speed++
  }

  speedDown() {
    if (this.speed > 1) this.speed--
  }
}
