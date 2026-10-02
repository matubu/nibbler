<script lang="ts">
  import { parseConfig, USAGE } from './lib/config'
  import { Game } from './lib/game.svelte'
  import Nibbler from './Nibbler.svelte'

  let game: Game | null = null
  let error = ''

  try {
    game = new Game(parseConfig(location.search), innerWidth, innerHeight)
  } catch (e) {
    error = (e as Error).message
  }
</script>

{#if game}
  <Nibbler {game} />
{:else}
  <main class="flex h-screen flex-col items-center justify-center gap-6 bg-[#0E183D] p-8 text-white">
    <p class="font-sigmar text-2xl text-red-400">Error: {error}</p>
    <pre class="font-mono text-sm">{USAGE}</pre>
  </main>
{/if}
