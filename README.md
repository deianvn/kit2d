# kit2d

## SpriteBatch

`SpriteBatch` buffers textured quads and submits their triangles together with
SDL's geometry renderer. Consecutive draws using the same texture, including
different atlas regions, share one submission. Texture changes, a full buffer,
blend-state changes, `flush()`, and `end()` submit pending geometry. Draw order is
preserved; textures are not sorted. Draw calls copy vertex data into the batch;
there is no queue of sprites to process later.

```cpp
kit2d::SpriteBatch batch { 1000 };
kit2d::TextureView region { texture, kit2d::Rect { 20, 20, 50, 50 } };

renderer.setColor(0, 0, 0, 255);
renderer.clear();
batch.begin(renderer);
batch.draw(texture, 10.0f, 10.0f);
batch.draw(region, 100.0f, 10.0f);
batch.setColor(0.0f, 1.0f, 0.0f, 1.0f);
batch.draw(region, 160.0f, 10.0f, 100.0f, 100.0f);
batch.end();
renderer.present();
```

`begin()` without an argument uses the active SDL renderer. All `draw()` calls
and `Sprite::draw(batch)` calls must occur between `begin()` and `end()`.
Incorrect lifecycle calls throw `std::logic_error`. `flush()` submits geometry
without ending the pass. `end()` does **not** present the frame.
Do not mix other renderer draws or change the render target, viewport, or clip
rectangle during a pass. These remain under the caller's control.

### Textures, regions, and transforms

`TextureView` is kit2d's equivalent of a texture region: it refers to a texture
and a pixel rectangle within it, without owning the texture. Keep textures alive
until the batch has ended.

The draw overloads support whole textures, pixel source rectangles, texture
views, explicit destination sizes, horizontal/vertical source flips, and
normalized UV rectangles. Transformed draws take a local origin, destination
width/height, independent X/Y scales, and rotation in degrees:

```cpp
batch.draw(region, x, y, originX, originY, width, height, scaleX, scaleY, rotation);
batch.draw(texture, x, y, width, height, srcX, srcY, srcWidth, srcHeight, flipX, flipY);
batch.draw(texture, x, y, width, height, u, v, u2, v2);
```

Unlike libGDX's default projection, coordinates retain SDL's **top-left origin
with Y increasing downward**. Positive rotation is clockwise. Geometry preserves
floating-point positions. Dimensions must be non-negative and UV coordinates
must be in `[0, 1]`; reversed UV endpoints flip the image.

For arbitrary quadrilaterals, use
`draw(texture, const std::vector<SDL_Vertex>& vertices, offset, count)`.
Offsets/counts are in vertices, and the count must be a multiple of four.
Each group uses top-left, top-right, bottom-right, bottom-left corner order
(indices `0,1,2,2,3,0`), with positions, normalized UVs, and byte RGBA colors.
Raw vertex colors are used directly rather than multiplied by the batch tint.

### Tinting and blending

`setColor(r, g, b, a)` takes finite components in `[0, 1]` and defaults to opaque
white. Tint is captured per quad, so changing it does not flush the batch or
recolor previous draws. `Sprite` uses its own tint rather than the batch tint.
Texture color/alpha modulation does not affect geometry draws.

Blending is enabled by default. `disableBlending()` makes draws replace
destination pixels; `enableBlending()` restores alpha compositing. Changing this
state flushes queued geometry first. Texture blend modes are restored after
submission so immediate drawing outside the batch is unaffected.

### Sprites and capacity

Sprites support `setPosition()`, `setSize()`, `setOrigin()`, `setScale()`,
`setRotation()`, and `setColor()`. `sprite.draw(batch)` captures the current frame
and geometry. Transformed vertices are cached until properties or animation
frames change, including direct edits to the existing public `position`.
An explicit size is retained when replacing the texture sequence; otherwise the
current frame's region determines the size.

Capacity defaults to **1000** quads and must be between **1 and 8191**.
`renderCalls` counts geometry submissions since the last `begin()`.
`maxSpritesInBatch` records the largest submission over the batch's lifetime and
can be reset to zero to start measuring again. Sharing an atlas and drawing
regions consecutively reduces texture changes and submission counts.

## Sprite animation

Sprites can draw a single `TextureView` or animate a sequence of views, including
regions of a texture atlas:

```cpp
kit2d::Sprite sprite { std::vector<kit2d::TextureView> { idle1, idle2, idle3 }, 8.0f };
sprite.position = kit2d::Vec2 { 100.0f, 50.0f };
sprite.loop = false; // Hold the last frame instead of looping (the default).
```

The frame rate defaults to 12 frames per second and can be changed with
`setFrameRate()`. It must be finite and greater than zero. Changing it preserves
progress through the current frame. `getFrameIndex()` returns the zero-based
current frame.

`Stage` forwards the window's delta time **in seconds** to `Scene::update()`.
Update sprites or actors there, independently of rendering. `SpriteBatch` only
draws; it never advances animation, including when a sprite is drawn more than
once. Actors can therefore continue animating even when they are not drawn.

`Stage` owns the begin/end pass and presents the frame. Call
`sprite.draw(spriteBatch)` or `spriteBatch.draw()` in `Scene::render()` without
another begin/end pair. Without `Stage`, call `sprite.update(deltaTime)` from
your own update loop and draw it between `begin()`/`end()`.
`sprite.draw(renderer)` remains available for immediate, unbatched drawing.
Delta time must be finite and non-negative. Invalid rates or delta times throw
`std::invalid_argument`. Updates catch up across multiple frames after a slow tick
and preserve fractional-frame progress.

`restart()` returns to the first frame and clears fractional-frame progress,
without changing the sprite's configuration or position.
`setTextureViews()` replaces the sequence and restarts it from the first frame;
`setTextureView()` switches back to a static sprite. Empty sequences draw nothing,
and single-view sprites remain static. Views do not own their textures, so keep
the underlying textures alive while the sprite uses them.

## Actors

`Actor` owns its current sprite and provides `act(deltaTime)` and
`draw(spriteBatch)`, suitable for characters that select different animations
for movement directions:

```cpp
kit2d::Sprite walkLeft { leftFrames, 8.0f };
kit2d::Sprite walkRight { rightFrames, 8.0f };
kit2d::Actor character { walkRight };
character.setPosition(100.0f, 50.0f);

if (facingLeft != wasFacingLeft) {
    character.setSprite(facingLeft ? walkLeft : walkRight);
    wasFacingLeft = facingLeft;
}
```

`setSprite()` copies the supplied sprite, restarts its animation, and preserves
the actor's position. Call it when the direction or animation changes, not on
every update, to avoid restarting every tick. Actors do not share animation state
with their source sprites or other actors. `getSprite()` provides mutable or const
access to the current sprite; `getPosition()` returns its position.

Call `character.act(deltaTime)` from `Scene::update()` and
`character.draw(spriteBatch)` from `Scene::render()`. `act()` advances the current
sprite; draw only submits its current frame to the batch. Override `Actor::act()`
for movement or direction selection and call `Actor::act(deltaTime)` to advance
animation. `prepare(AssetManager&)` remains the resource-loading hook.
The bricks example uses this update/draw separation.

## Code style

Use four spaces for indentation and a 100-column line limit. Function declarations
and definitions with more than two parameters, or signatures that do not fit on
one line, use one parameter per line with the closing parenthesis on its own line:

```cpp
void f(
    int a,
    int b,
    int c
) {
}
```

The parameter-count rule does not apply to function calls. Formatting settings
are included for clang-format 23 or newer, cmake-format, and EditorConfig.

## Playable example

The `bricks` example is **Neon Break**, an original single-level brick breaker.
Clear the six-row field by bouncing the ball off the paddle. The point of impact
on the paddle controls the rebound angle, and breaking bricks gradually speeds
up the ball. A miss costs one of three lives.

| Control | Action |
|---|---|
| Mouse, Left/Right, or A/D | Move the paddle |
| Space or left click | Launch the ball; replay after winning or losing |
| P | Pause or resume |
| Space while paused | Resume |
| R | Restart the level, score, and lives |
| Escape | Quit |

Bricks use looping two-texture `Sprite` animations at 4 FPS, advanced by
`Actor::act(deltaTime)`. Silver armor takes two hits. Its first hit replaces the
sprite's texture sequence with a pink damaged animation blinking at 8 FPS.
The palette, beveled textures, ball, paddle, and pixel lettering are generated
locally; no external artwork or font packages are needed.

The level layout is `example/assets/level.txt`: `S` is armor; `O`, `R`, `G`, `C`,
and `V` are single-hit colored bricks; spaces and `0` are empty cells.

## Building

With SDL2 **2.0.18 or newer** and SDL2_image installed:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

To build and run only the example from the repository root:

```sh
mkdir -p build
cd build
cmake ..
cmake --build . --target bricks
./bricks
```

Run from the build directory so the game can read the copied `assets/level.txt`.