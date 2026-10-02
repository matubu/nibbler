const sounds = {
  music: new Audio('/sounds/music.wav'),
  eat: new Audio('/sounds/eat.wav'),
  die: new Audio('/sounds/die.wav'),
}

type Sound = keyof typeof sounds

let enabled = true
// Looping sounds the browser refused to start before any user interaction
const blocked = new Set<Sound>()

export function setAudioEnabled(value: boolean) {
  enabled = value
}

export function play(name: Sound, loop = false) {
  if (!enabled) return
  const sound = sounds[name]
  sound.pause()
  sound.currentTime = 0
  sound.loop = loop
  sound.play().catch(() => loop && blocked.add(name))
}

export function stop(name: Sound) {
  blocked.delete(name)
  sounds[name].pause()
  sounds[name].currentTime = 0
}

// Browsers block autoplay until the first user gesture: start blocked loops then
function unlock() {
  for (const name of blocked) play(name, true)
  blocked.clear()
}
addEventListener('keydown', unlock)
addEventListener('pointerdown', unlock)
