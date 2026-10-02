<script lang="ts">
  import { saveConfig, type Config } from './lib/config'
  import { Game } from './lib/game.svelte'
  import Nibbler from './Nibbler.svelte'
  import StartMenu from './StartMenu.svelte'

  let game = $state<Game | null>(null)

  function start(config: Config) {
    saveConfig(config)
    game = new Game(config, innerWidth, innerHeight)
  }
</script>

{#if game}
  <Nibbler {game} onquit={() => (game = null)} />
{:else}
  <StartMenu onstart={start} />
{/if}
