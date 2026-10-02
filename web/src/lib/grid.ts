// In points, the SFML window renders tiles at 2x on retina screens
export const TILE_SIZE = 30

// Number of tiles fitting in a length in points, within the native game limits
export const tilesIn = (length: number) => Math.min(255, Math.max(10, Math.floor(length / TILE_SIZE)))

export type Vec2 = { x: number; y: number }

export const Tile = { Empty: 0, Snake: 1, Food: 2 } as const
export type Tile = (typeof Tile)[keyof typeof Tile]

export class Grid {
  readonly width: number
  readonly height: number
  readonly cells: Uint8Array

  constructor(width: number, height: number, cells = new Uint8Array(width * height)) {
    this.width = width
    this.height = height
    this.cells = cells
  }

  clone() {
    return new Grid(this.width, this.height, this.cells.slice())
  }

  isOutOfBounds({ x, y }: Vec2) {
    return x < 0 || x >= this.width || y < 0 || y >= this.height
  }

  isFree(pos: Vec2) {
    return !this.isOutOfBounds(pos) && this.get(pos) !== Tile.Snake
  }

  get({ x, y }: Vec2): Tile {
    return this.cells[y * this.width + x] as Tile
  }

  set({ x, y }: Vec2, tile: Tile) {
    this.cells[y * this.width + x] = tile
  }
}
