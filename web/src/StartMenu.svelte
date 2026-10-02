<script lang="ts">
  import { tick } from 'svelte'
  import { CONTROLS, loadConfig, MAX_PLAYERS, MAX_SPEED, type Config, type Control } from './lib/config'

  let { onstart }: { onstart: (config: Config) => void } = $props()

  let config = $state(loadConfig())

  const LABELS: Record<Control, string> = { arrows: 'Arrows', wasd: 'WASD', bot: 'Bot', off: 'Off' }

  // The players, then an empty slot to add one more
  const slots = $derived<Control[]>(config.players.length < MAX_PLAYERS ? [...config.players, 'off'] : config.players)
  const canStart = $derived(config.players.length > 0)

  // A set of keys drives a single player: taking it removes the player who had it
  function choose(slot: number, control: Control) {
    const players: Control[] = config.players.map((c, i) => (i === slot ? control : c))
    if (slot === players.length) {
      players.push(control)
      // Keep the new empty slot in view
      tick().then(() => list.lastElementChild?.scrollIntoView({ block: 'nearest' }))
    }
    config.players = players.filter(
      (c, i) => c !== 'off' && (i === slot || c !== control || (c !== 'arrows' && c !== 'wasd')),
    )
  }

  let list: HTMLDivElement

  const speedPercent = $derived(((config.speed - 1) / (MAX_SPEED - 1)) * 100)

  function start() {
    if (canStart) onstart($state.snapshot(config))
  }

  function onkeydown(event: KeyboardEvent) {
    if (event.key !== 'Enter') return
    event.preventDefault() // a focused button would also get clicked
    start()
  }
</script>

<svelte:window {onkeydown} />

{#snippet link(href: string, label: string)}
  <a {href} target="_blank" rel="noreferrer" class="font-semibold text-white hover:underline">{label}</a>
{/snippet}

{#snippet credit(name: string)}
  {@render link(`https://github.com/${name}`, `@${name}`)}
{/snippet}

{#snippet key(name: string)}
  <kbd class="rounded-md border border-white/20 bg-white/10 px-1.5 py-0.5 font-mono text-xs text-white">{name}</kbd>
{/snippet}

<main class="flex min-h-screen flex-col items-center justify-center gap-6 bg-[#09102A] p-4 text-white select-none sm:p-8">
  <form
    class="flex w-full max-w-lg flex-col gap-7 rounded-3xl border border-white/10 bg-[#0E183D] p-6 shadow-2xl sm:p-8"
    onsubmit={(event) => {
      event.preventDefault()
      start()
    }}
  >
    <h1 class="flex items-center justify-center gap-3 font-sigmar text-5xl">
      <img src="/assets/blue/head.png" alt="" class="size-12 [image-rendering:pixelated]" />
      Nibbler
    </h1>

    <section class="flex flex-col gap-2">
      <h2 class="flex justify-between text-sm font-semibold tracking-wide text-white/70 uppercase">
        Players <span class="text-white">{config.players.length} / {MAX_PLAYERS}</span>
      </h2>
      <div class="-mr-2 flex max-h-[40vh] flex-col gap-2 overflow-y-auto pr-2" bind:this={list}>
        {#each slots as control, player (player)}
          <div class="flex items-center justify-between gap-3">
            <span class="font-medium {control === 'off' ? 'text-white/40' : ''}">Player {player + 1}</span>
            <div class="flex rounded-xl bg-black/25 p-1">
              {#each CONTROLS as option (option)}
                <button
                  type="button"
                  class="w-16 rounded-lg py-1.5 text-sm font-semibold transition sm:w-20 {control === option
                    ? option === 'off'
                      ? 'bg-white/15 text-white'
                      : 'bg-white text-[#0E183D]'
                    : 'text-white/60 hover:text-white'}"
                  onclick={() => choose(player, option)}
                >
                  {LABELS[option]}
                </button>
              {/each}
            </div>
          </div>
        {/each}
      </div>
    </section>

    <section class="flex flex-col gap-5">
      <label class="flex flex-col gap-3">
        <span class="flex justify-between text-sm font-semibold tracking-wide text-white/70 uppercase">
          Speed <span class="text-white">{config.speed}</span>
        </span>
        <input
          type="range"
          min="1"
          max={MAX_SPEED}
          bind:value={config.speed}
          class="slider h-4 w-full cursor-pointer appearance-none rounded-full"
          style:background="linear-gradient(to right, white {speedPercent}%, rgb(0 0 0 / 0.25) {speedPercent}%)"
        />
      </label>

      <div class="flex items-center justify-between">
        <span class="text-sm font-semibold tracking-wide text-white/70 uppercase">Music</span>
        <div class="flex rounded-xl bg-black/25 p-1">
          {#each [true, false] as on (on)}
            <button
              type="button"
              class="w-16 rounded-lg py-1.5 text-sm font-semibold transition sm:w-20 {config.music === on
                ? on
                  ? 'bg-white text-[#0E183D]'
                  : 'bg-white/15 text-white'
                : 'text-white/60 hover:text-white'}"
              onclick={() => (config.music = on)}
            >
              {on ? 'On' : 'Off'}
            </button>
          {/each}
        </div>
      </div>
    </section>

    <button
      type="submit"
      disabled={!canStart}
      class="rounded-2xl bg-white py-3 font-sigmar text-2xl text-[#0E183D] transition enabled:hover:scale-[1.02] disabled:opacity-40"
    >
      {canStart ? 'Play' : 'Add a player'}
    </button>

    <p class="flex flex-wrap justify-center gap-x-4 gap-y-2 text-sm text-white/70">
      <span>{@render key('+')} {@render key('-')} Speed</span>
      <span>{@render key('Space')} Rainbow</span>
      <span>{@render key('R')} Restart</span>
      <span>{@render key('Esc')} Menu</span>
    </p>

  </form>

  <footer class="flex flex-col items-center gap-3 text-sm text-white/60">
    <dl class="grid grid-cols-[auto_auto] gap-x-3 gap-y-1">
      <dt class="text-right">Code</dt>
      <dd>{@render credit('Edracoon')} & {@render credit('matubu')}</dd>
      <dt class="text-right">Music</dt>
      <dd>{@render credit('dsamain')}</dd>
      <dt class="text-right">Art</dt>
      <dd>{@render credit('matubu')}, homemade</dd>
    </dl>
    {@render link('https://github.com/matubu/nibbler', 'Source on GitHub')}
  </footer>
</main>

<style>
  .slider::-webkit-slider-thumb {
    appearance: none;
    width: 1.75rem;
    height: 1.75rem;
    border-radius: 9999px;
    background: white;
    border: 4px solid #0e183d;
    box-shadow: 0 0 0 2px white;
  }
  .slider::-moz-range-thumb {
    width: 1.25rem;
    height: 1.25rem;
    border-radius: 9999px;
    background: white;
    border: 4px solid #0e183d;
    box-shadow: 0 0 0 2px white;
  }
</style>
