<script lang="ts">
  import { onMount } from 'svelte'
  import type { Game } from './lib/game.svelte'
  import { TILE_SIZE } from './lib/grid'
  import { foodTexture, snakeSprites, TEXTURES, type Sprite } from './lib/sprites'

  let { game }: { game: Game } = $props()

  // Same sizes as the SFML window, in points (it renders them at 2x on retina screens)
  const HELP_FONT_SIZE = 15
  const TEXT_FONT_SIZE = 24
  const TITLE_FONT_SIZE = 48

  // SigmarOne metrics (in em): SFML puts the baseline `size` below the text position,
  // CSS puts it `ascent` below the top of a line box
  const FONT_ASCENT = 1.172
  const LINE_HEIGHT = 1.638
  const textTop = (y: number, size: number) => y + size * (1 - FONT_ASCENT)

  let innerWidth = $state(window.innerWidth)
  let innerHeight = $state(window.innerHeight)
  let devicePixelRatio = $state(window.devicePixelRatio)

  const gridWidth = $derived(game.width * TILE_SIZE)
  const gridHeight = $derived(game.height * TILE_SIZE)
  // Only grids forced bigger than the screen through the URL get scaled down
  const scale = $derived(Math.min(1, innerWidth / gridWidth, innerHeight / gridHeight))
  const screenWidth = $derived(innerWidth / scale)
  const screenHeight = $derived(innerHeight / scale)

  let rainbowMode = $state(false)

  const move = (snakeId: number, x: number, y: number) => game.changeDirection(snakeId, { x, y })
  // WASD drives the second player, or the first one when there is no other human
  const wasd = $derived(game.config.multiplayer && !game.config.bot ? 1 : 0)

  // Matched on KeyboardEvent.code (physical key, so WASD is ZQSD on AZERTY), then on KeyboardEvent.key
  const shortcuts: Record<string, () => void> = {
    ArrowUp: () => move(0, 0, -1),
    ArrowDown: () => move(0, 0, 1),
    ArrowLeft: () => move(0, -1, 0),
    ArrowRight: () => move(0, 1, 0),
    KeyW: () => move(wasd, 0, -1),
    KeyS: () => move(wasd, 0, 1),
    KeyA: () => move(wasd, -1, 0),
    KeyD: () => move(wasd, 1, 0),
    Space: () => (rainbowMode = !rainbowMode),
    '+': () => game.speedUp(),
    '-': () => game.speedDown(),
    r: () => game.reset(),
  }

  function onkeydown(event: KeyboardEvent) {
    if (event.ctrlKey || event.metaKey || event.altKey) return
    const action = shortcuts[event.code] ?? shortcuts[event.key.toLowerCase()]
    if (!action) return
    event.preventDefault()
    action()
  }

  const textures = new Map(TEXTURES.map((src) => [src, Object.assign(new Image(), { src })]))
  let canvas: HTMLCanvasElement
  let loaded = $state(false)

  // Like SFML: sprites centered on their tile, rotated by quarter turns, without smoothing
  function drawSprite(ctx: CanvasRenderingContext2D, { src, x, y, rot }: Sprite) {
    ctx.save()
    ctx.translate((x + 0.5) * TILE_SIZE, (y + 0.5) * TILE_SIZE)
    ctx.rotate((rot * Math.PI) / 2)
    ctx.drawImage(textures.get(src)!, -TILE_SIZE / 2, -TILE_SIZE / 2, TILE_SIZE, TILE_SIZE)
    ctx.restore()
  }

  $effect(() => {
    if (!loaded) return
    const ctx = canvas.getContext('2d')!
    ctx.setTransform(devicePixelRatio, 0, 0, devicePixelRatio, 0, 0)
    ctx.imageSmoothingEnabled = false
    ctx.clearRect(0, 0, gridWidth, gridHeight)
    for (const snake of game.snakes) {
      for (const part of snakeSprites(snake, rainbowMode)) drawSprite(ctx, part)
    }
    drawSprite(ctx, { src: foodTexture(game.food), ...game.food, rot: 0 })
  })

  onMount(() => {
    let frame = 0
    Promise.all([...textures.values()].map((img) => img.decode())).then(() => {
      loaded = true

      // Update every 1/speed seconds, possibly several times per frame at high speeds
      let nextUpdate = performance.now()
      frame = requestAnimationFrame(function loop(now) {
        if (now - nextUpdate > 1000) nextUpdate = now // don't catch up after the tab was hidden
        while (now > nextUpdate) {
          game.update()
          nextUpdate += 1000 / game.speed
        }
        frame = requestAnimationFrame(loop)
      })
    })
    return () => cancelAnimationFrame(frame)
  })
</script>

<svelte:window
  bind:innerWidth
  bind:innerHeight
  bind:devicePixelRatio
  {onkeydown}
  onresize={() => game.fit(window.innerWidth, window.innerHeight)}
/>

{#snippet text(content: string, y: number, size: number, className: string)}
  <p
    class="absolute whitespace-pre {className}"
    style:top="{textTop(y, size)}px"
    style:font-size="{size}px"
    style:line-height={LINE_HEIGHT}
  >
    {content}
  </p>
{/snippet}

<main class="h-screen overflow-hidden bg-[#09102A] font-sigmar text-white select-none">
  <div
    class="pointer-events-none relative origin-top-left"
    style:width="{screenWidth}px"
    style:height="{screenHeight}px"
    style:scale
  >
    <canvas
      bind:this={canvas}
      width={gridWidth * devicePixelRatio}
      height={gridHeight * devicePixelRatio}
      class="absolute bg-[#0E183D]"
      style:left="{Math.floor((screenWidth - gridWidth) / 2)}px"
      style:top="{Math.floor((screenHeight - gridHeight) / 2)}px"
      style:width="{gridWidth}px"
      style:height="{gridHeight}px"
    ></canvas>

    {@render text(`${game.scoreText}\n[+/-] Speed: ${game.speed}`, 5, HELP_FONT_SIZE, 'left-[7.5px]')}

    {#if game.gameOver}
      <img src="/assets/death_overlay.png" alt="" class="absolute inset-0 size-full" />
      {@render text('Game Over!', screenHeight / 2 - TITLE_FONT_SIZE - TEXT_FONT_SIZE, TITLE_FONT_SIZE, 'inset-x-0 text-center')}
      {@render text('Press R to Restart', screenHeight / 2 + TEXT_FONT_SIZE / 2, TEXT_FONT_SIZE, 'inset-x-0 text-center')}
    {/if}
  </div>
</main>
