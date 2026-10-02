import { play, setAudioEnabled, stop } from './audio'
import { botDirection } from './bot'
import type { Config } from './config'
import { Grid, Tile, tilesIn, type Vec2 } from './grid'

export type SnakePart = Vec2 & { isEating: boolean }

// Holding R would reset on every key repeat
const RESET_COOLDOWN_MS = 100

export class Snake {
  parts = $state<SnakePart[]>([])
  isDead = $state(false)
  score = $state(0)
  direction: Vec2 = { x: 0, y: -1 }
  isBot: boolean

  // x and y from the center of the snake
  constructor(x: number, y: number, isBot = false) {
    this.parts = [-2, -1, 0, 1].map((dy) => ({ x, y: y + dy, isEating: false }))
    this.isBot = isBot
  }

  // Returns true if the snake has eaten
  update(grid: Grid) {
    if (this.isBot) {
      this.direction = botDirection(grid, this.parts[0])
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

  // Resize the grid to fill the viewport, unless its size was set in the URL.
  // The game goes on: snakes left outside die on their next move.
  fit(viewportWidth: number, viewportHeight: number) {
    this.width = this.config.width ?? tilesIn(viewportWidth)
    this.height = this.config.height ?? tilesIn(viewportHeight)
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

    const x = Math.floor(this.width / 2)
    const y = Math.floor(this.height / 2)

    this.gameOver = false
    const { multiplayer, bot } = this.config
    this.snakes = multiplayer ? [new Snake(x - 3, y), new Snake(x + 3, y, bot)] : [new Snake(x, y, bot)]
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

      if (snake.update(this.grid())) {
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

  changeDirection(snakeId: number, direction: Vec2) {
    const snake = this.snakes[snakeId]
    if (!snake) return

    const [head, neck] = snake.parts
    if (head.x + direction.x === neck.x && head.y + direction.y === neck.y) return
    snake.direction = direction
  }

  speedUp() {
    this.speed++
  }

  speedDown() {
    if (this.speed > 1) this.speed--
  }
}
