export const CONTROLS = ['arrows', 'wasd', 'bot', 'off'] as const
export type Control = (typeof CONTROLS)[number]

export const MAX_PLAYERS = 25
export const MAX_SPEED = 60

export type Config = {
  // One snake per entry, never 'off'
  players: Control[]
  speed: number
  music: boolean
}

const STORAGE_KEY = 'nibbler-config'

const defaultConfig = (): Config => ({
  players: ['arrows'],
  speed: 20,
  music: true,
})

// The last options picked in the start menu, remembered across visits
export function loadConfig(): Config {
  const config = defaultConfig()
  try {
    const saved = JSON.parse(localStorage.getItem(STORAGE_KEY) ?? '{}')
    const { players, speed, music } = saved
    if (
      Array.isArray(players) &&
      players.length > 0 &&
      players.length <= MAX_PLAYERS &&
      players.every((control) => control !== 'off' && CONTROLS.includes(control))
    ) {
      config.players = players
    }
    if (Number.isInteger(speed) && speed >= 1 && speed <= MAX_SPEED) config.speed = speed
    if (typeof music === 'boolean') config.music = music
  } catch {
    // Keep the defaults
  }
  return config
}

export function saveConfig(config: Config) {
  localStorage.setItem(STORAGE_KEY, JSON.stringify(config))
}
