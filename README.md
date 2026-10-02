# Nibbler

🐍 A snake game with an graphical interface you can change at run time

![](./screenshots/gameplay.png)
![](./screenshots/gameover.png)

## Web version

A standalone Svelte + Vite + Tailwind port lives in [`web/`](./web):

```sh
cd web && npm install && npm run dev
```

The grid fills the window. A start menu sets up to 25 players (arrows, WASD or bot), the speed and music, and remembers them; Esc goes back to it.

### Deployment

The web version is deployed as Cloudflare Worker static assets at
[snake.mathias.ninja](https://snake.mathias.ninja). The custom domain is configured in
`wrangler.jsonc` and serves the build output from `web/dist`.

Cloudflare Workers Builds watches the `main` branch and deploys every new commit with
`npx wrangler deploy`, which builds the web app first (`build.command` in `wrangler.jsonc`).

## Contributors
 - Code: [@Edracoon](https://github.com/Edracoon) & [@matubu](https://github.com/matubu)
 - Graphical assets: [@matubu](https://github.com/matubu)
 - Music: [@dsamain](https://github.com/dsamain)
