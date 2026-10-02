import type { Snake } from './game.svelte'
import type { Vec2 } from './grid'

export type Sprite = { src: string; x: number; y: number; rot: number }

const PACKS = ['blue', 'orange', 'green']
const PARTS = ['head', 'body_straight', 'body_turn', 'tail']
const FOOD_VARIANTS = 5

const texture = (name: string) => `/assets/${name}.png`

export const TEXTURES = [
  ...PACKS.flatMap((pack) => [
    ...PARTS.flatMap((part) => [texture(`${pack}/${part}`), texture(`${pack}/${part}_eating`)]),
    texture(`${pack}/head_dead`),
  ]),
  ...Array.from({ length: FOOD_VARIANTS }, (_, i) => texture(`food-${i}`)),
  texture('death_overlay'),
]

// srand(seed); rand() from macOS libc (Park-Miller), so food variants match the native game
function seededRand(seed: number) {
  const state = seed || 123459876
  let x = 16807 * (state % 127773) - 2836 * Math.floor(state / 127773)
  if (x < 0) x += 0x7fffffff
  return x
}

export function foodTexture({ x, y }: Vec2) {
  return texture(`food-${seededRand((x << 16) | y) % FOOD_VARIANTS}`)
}

// Rotation (in quarter turns) of a tile going from a to b
function orientation(a: Vec2, b: Vec2) {
  if (b.y > a.y) return 0
  if (a.x > b.x) return 1
  if (b.y !== a.y) return 2
  return 3
}

const ROTATIONS: Vec2[] = [
  { x: 0, y: -1 },
  { x: 1, y: 0 },
  { x: 0, y: 1 },
  { x: -1, y: 0 },
]

// Rotation of a turn tile b between a and c, depending on whether it turns left or right
function turnOrientation(a: Vec2, b: Vec2, c: Vec2) {
  const rot = orientation(a, b)
  const left = ROTATIONS[(rot + 3) % 4]
  return c.x === b.x + left.x && c.y === b.y + left.y ? rot : rot + 1
}

export function snakeSprites(snake: Snake, rainbowMode: boolean): Sprite[] {
  const parts = snake.parts

  return parts.map((part, i) => {
    const prev = parts[i - 1]
    const next = parts[i + 1]
    let name: string
    let rot: number

    if (i === 0) {
      name = 'head'
      rot = orientation(part, next)
    } else if (i === parts.length - 1) {
      name = 'tail'
      rot = orientation(prev, part)
    } else if (prev.x === next.x || prev.y === next.y) {
      name = 'body_straight'
      rot = orientation(prev, part)
    } else {
      name = 'body_turn'
      rot = turnOrientation(prev, part, next)
    }

    if (snake.isDead && name === 'head') {
      name += '_dead'
    } else if (part.isEating) {
      name += '_eating'
    }

    const pack = PACKS[(rainbowMode ? i : snake.score) % PACKS.length]
    return { src: texture(`${pack}/${name}`), x: part.x, y: part.y, rot }
  })
}
