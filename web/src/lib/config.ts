export type Config = {
  // null: fit the screen
  width: number | null
  height: number | null
  multiplayer: boolean
  bot: boolean
  speed: number
  music: boolean
}

export const DEFAULT_SPEED = 20

export const USAGE = `Usage:
   /?width=<width>&height=<height>[&options]
Options:
   width=<n>       width in cell (default: fit the screen)
   height=<n>      height in cell (default: fit the screen)
   no-music        disable music
   multiplayer     enable multiplayer mode
   bot             enable bot mode
   speed=<n>       the speed at which the snakes move`

// The URL query string replaces the native command line arguments
export function parseConfig(search: string): Config {
  const params = new URLSearchParams(search)
  const size = (name: string) => (params.has(name) ? Number(params.get(name)) : null)
  const width = size('width')
  const height = size('height')
  const speed = Number(params.get('speed') ?? DEFAULT_SPEED)

  const isSize = (n: number | null) => n === null || (Number.isInteger(n) && n >= 10 && n < 256)
  if (!isSize(width) || !isSize(height)) {
    throw new Error('width and height must be >=10 and <256')
  }
  if (!Number.isInteger(speed) || speed < 1) {
    throw new Error('speed must be a valid integer greater than 0')
  }

  return {
    width,
    height,
    speed,
    multiplayer: params.has('multiplayer'),
    bot: params.has('bot'),
    music: !params.has('no-music'),
  }
}
