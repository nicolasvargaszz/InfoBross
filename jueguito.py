#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import pygame
import sys
import os
from pygame.locals import *

###############################################################################
#                            GLOBAL SETTINGS
###############################################################################
SCREEN_WIDTH  = 800
SCREEN_HEIGHT = 600
FPS           = 60

TILE_SIZE     = 32

# Player physics
PLAYER_MOVE_SPEED   = 4
PLAYER_JUMP_SPEED   = 12
GRAVITY             = 0.6
MAX_FALL_SPEED      = 12

# Player stats
PLAYER_START_LIVES  = 3
INVULN_TIME_MS      = 1000  # 1s of invulnerability after a hit

# Enemy stats
ENEMY_SPEED         = 1.5
ENEMY_DAMAGE        = 1  # lose 1 HP if collided

# Filenames for assets
FILE_BG_FAR         = "./image/background.jpg"
FILE_BG_NEAR        = "./image/background.jpg"
FILE_PLAYER         = "./image/mario.png"
FILE_ENEMY          = "./image/goomba.png"
FILE_TILE_GROUND    = "./image/tile_ground.png"
FILE_TILE_BRICK     = "./image/tile_brick.png"
FILE_TILE_QUESTION  = "./image/tile_question.jpg"
FILE_TILE_PIPE      = "./image/tile_pipe.png"
FILE_TILE_SKY       = "./image/sky_pile.png"

# Game states
STATE_PLAYING  = 0
STATE_GAMEOVER = 1
STATE_WIN      = 2

# Colors
WHITE = (255,255,255)
RED   = (255,0,0)
GREEN = (0,255,0)
BLACK = (0,0,0)

###############################################################################
#   FIVE LEVELS, each quite large (20 rows x 100 columns). Characters:
#   '#' = ground, 'B' = brick, '?' = question block, 'P' = pipe, '.' = sky
#   'G' = goomba, 'M' = player spawn
#
#   The user can't kill enemies. Collisions reduce HP.
#   Enemies chase horizontally, won't fall off edges if placed properly.
###############################################################################

LEVEL_1 = [
    "....................................................................................................",
    "....................................................................................................",
    "........................?.......................B....................................................",
    "...........................................?????.....................................................",
    ".....................M.........................................????????..............................",
    "##########.....###########..##########....##########..#######..######################################...",
    "##########........###########.................##########.......##########............#######........",
    "##########........###########.................##########.......#######GG............#######........",
    "#######GG#........#####GG####.................##########.......##########............#######........",
    "##########........###########.................##########.......##########............#######........",
    "##########........#####????##.................#####????#.......##########............#######........",
    "##########........###########.................##########.......##########............#######........",
    "##########........###########.................##########.......##########............#######........",
    "##########........###########.................##########.......##########............#######........",
    "##########........###########.................##########.......##########............#######........",
    "##########........###########.................##########.......##########............#######........",
    "##########........###########.................##########.......##########............#######........",
    "##########........###########.................##########.......##########............#######........",
    "##########........###########.................##########.......##########............#######........",
    "####################################################################################################",
]

LEVEL_2 = [
    "....................................................................................................",
    ".....................????.......................GG...................................................",
    "..........B......................................GG..........B...........???...........................",
    "..M......GG.......GG...GG..............GG.........GG......??????...............................................",
    "###########.###########.....###############..###############.....###########.....#######..........",
    "###########...###########.....###############..###############.....###########.....#######..........",
    "###########...##########.....################..###############.....###########.....#######..........",
    "###########...############...###############..###############.....###########.....#######..........",
    "###########...############...###############..###############.....###########.....#######..........",
    "###########...############...###############..###############.....###########.....#######..........",
    "###########...############...###############..###############.....###########.....#######..........",
    "###########...####???####.....####???#####....#####GG########.....###########.....#######..........",
    "###########...############...###############..###############.....###########.....#######..........",
    "###########...############...###############..###############.....###########.....#######..........",
    "###########...############...###############..###############.....###########.....#######..........",
    "###########...############...###############..###############.....###########.....#######..........",
    "###########...############...###############..###############.....###########.....#######..........",
    "###########...############...###############..###############.....###########.....#######..........",
    "###########...############...###############..###############.....###########.....#######..........",
    "####################################################################################################",
]

LEVEL_3 = [
    "....................................................................................................",
    "..........................?.........................................................................",
    "....................................................................................................",
    "..........B........B..............?.........GG.........................???...........................",
    ".....................................................GG..............................................",
    ".....M.....................................GG.........................?????????......................",
    "###########.....########.....########.....#######....###########.....###########....#######.........",
    "###########.....########.....########.....#######....###########.....###########....#######.........",
    "###???#####.....##GG#####.....###???##.....##GG###....###???#####.....###???#####....#######.........",
    "###########.....########.....########.....#######....###########.....###########....#######.........",
    "###########.....########.....########.....#######....###########.....###########....#######.........",
    "###########.....########.....########.....#######....###########.....###########....#######.........",
    "###########.....###???##.....###???##.....#######....###???#####.....###???#####....#######.........",
    "###########.....########.....########.....#######....###########.....###########....#######.........",
    "###########.....########.....########.....#######....###########.....###########....#######.........",
    "###########.....##GG#####.....##GG#####...#######....###???#####.....###???#####....#######.........",
    "###########.....########.....########.....#######....###########.....###########....#######.........",
    "###########.....########.....########.....#######....###########.....###########....#######.........",
    "###########.....########.....########.....#######....###########.....###########....#######.........",
    "####################################################################################################",
]

LEVEL_4 = [
    "....................................................................................................",
    "..................................???.......................???.....................................",
    "............GG.................B......????..GG............B...................GG....................",
    "....................................................................................................",
    ".........M...............???...........######.............???........................................",
    "###########.....########.....########.....#######....###########.....###########....##...#######.....",
    "###########.....########.....########.....#######....###########.....###########........#######.....",
    "###???#####.....##GG#####.....###???##.....##GG###....###???#####.....###???#####........#######.....",
    "###########.....########.....########.....#######....###########.....###########........#######.....",
    "###########.....########.....########.....#######....###########.....###########........#######.....",
    "###########.....########.....########.....#######....###########.....###########........#######.....",
    "###########.....###???##.....###???##.....#######....###???#####.....###???#####........#######.....",
    "###########.....########.....########.....#######....###########.....###########........#######.....",
    "###########.....########.....########.....#######....###########.....###########........#######.....",
    "###########.....##GG#####.....##GG#####...#######....###???#####.....###???#####........#######.....",
    "###########.....########.....########.....#######....###########.....###########........#######.....",
    "###########.....########.....########.....#######....###########.....###########........#######.....",
    "###########.....########.....########.....#######....###########.....###########........#######.....",
    "###########.....########.....########.....#######....###########.....###########........#######.....",
    "####################################################################################################",
]

LEVEL_5 = [
    "....................................................................................................",
    "....................................................................................................",
    "......??..........GG...........P.......???........GG................B........???.....................",
    ".....?????.............................................???...........................................",
    "....M...............GG............GG...................GG..............GG.GG...................................",
    "###########...######????####..###########.....###############.....#########.......#######.........",
    "###########...###############..#############..###############.....###########.......#######.........",
    "###########...###############..#############..#####???#######.....###########.......#######.........",
    "###########...#############..############..###########........#########.......#######.........",
    "###########...###############..###############..###########.......###########.......#######.........",
    "###########...###############..###############..###########.......###########.......#######.........",
    "###########...#######???#####..#####???#######..#####???###.......#####???###.......#######.........",
    "###########...###############..###############..###########.......###########.......#######.........",
    "###########...###############..###############..###########.......###########.......#######.........",
    "###########...###############..###############..###########.......###########.......#######.........",
    "###########...###############..###############..###########.......###########.......#######.........",
    "###########...#############..############..###########........###########.......#######.........",
    "###########...###############..###############..###########.......###########.......#######.........",
    "###########...###############..###############..###########.......###########.......#######.........",
    "####################################################################################################",
]

ALL_LEVELS = [LEVEL_1, LEVEL_2, LEVEL_3, LEVEL_4, LEVEL_5]

###############################################################################
#                        INITIALIZE PYGAME
###############################################################################
pygame.init()
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption("Improved Mario-Like Game (No Enemy Kills)")
clock = pygame.time.Clock()

###############################################################################
#                  HELPER: Load image or create placeholder
###############################################################################
def load_image_or_placeholder(filename, width=None, height=None, fill=(255,0,255)):
    """
    Loads 'filename' if it exists; else, returns a colored surface placeholder.
    """
    if not os.path.exists(filename):
        surf = pygame.Surface((width if width else TILE_SIZE,
                               height if height else TILE_SIZE))
        surf.fill(fill)
        return surf
    else:
        img = pygame.image.load(filename).convert_alpha()
        if width and height:
            img = pygame.transform.scale(img, (width, height))
        return img

# ----------------------------------------------------------------------
# Load background images
bg_far    = load_image_or_placeholder(FILE_BG_FAR,  SCREEN_WIDTH, SCREEN_HEIGHT, (135,206,235))
bg_near   = load_image_or_placeholder(FILE_BG_NEAR, SCREEN_WIDTH, SCREEN_HEIGHT, (100,200,255))

# Load tile images
tile_ground    = load_image_or_placeholder(FILE_TILE_GROUND,   TILE_SIZE, TILE_SIZE, (128,64,0))
tile_brick     = load_image_or_placeholder(FILE_TILE_BRICK,    TILE_SIZE, TILE_SIZE, (150,75,0))
tile_question  = load_image_or_placeholder(FILE_TILE_QUESTION, TILE_SIZE, TILE_SIZE, (255,255,0))
tile_pipe      = load_image_or_placeholder(FILE_TILE_PIPE,     TILE_SIZE, TILE_SIZE, (0,255,0))
tile_sky       = load_image_or_placeholder(FILE_TILE_SKY,      TILE_SIZE, TILE_SIZE, (100,200,255))

# Load player/enemy sprites
player_surf = load_image_or_placeholder(FILE_PLAYER, TILE_SIZE, TILE_SIZE, (255,0,0))
enemy_surf  = load_image_or_placeholder(FILE_ENEMY,  TILE_SIZE, TILE_SIZE, (128,64,64))

###############################################################################
#                              TILE CLASS
###############################################################################
class Tile(pygame.sprite.Sprite):
    """
    A single tile that may or may not be solid. Characters:
    - '#' (ground) => tile_ground, solid
    - 'B' (brick) => tile_brick, solid
    - '?' (question) => tile_question, solid
    - 'P' (pipe) => tile_pipe, solid
    - '.' (sky) => tile_sky, not solid
    """
    def __init__(self, x, y, kind):
        super().__init__()
        self.kind = kind
        if kind == '#':
            self.image = tile_ground
            self.solid = True
        elif kind == 'B':
            self.image = tile_brick
            self.solid = True
        elif kind == '?':
            self.image = tile_question
            self.solid = True
        elif kind == 'P':
            self.image = tile_pipe
            self.solid = True
        else:
            self.image = tile_sky
            self.solid = False
        self.rect = self.image.get_rect(topleft=(x,y))

###############################################################################
#                           ENEMY CLASS
###############################################################################
class Enemy(pygame.sprite.Sprite):
    """
    A goomba that chases the player horizontally but won’t walk off edges.
    Colliding with it costs 1 HP for the player. The enemy can’t be killed.
    """
    def __init__(self, x, y):
        super().__init__()
        self.image = enemy_surf
        self.rect = self.image.get_rect(topleft=(x, y))
        self.vx = 0
        self.vy = 0

    def update(self, tile_sprites, player):
        """
        1) Decide horizontal direction to approach the player (chase).
        2) Check if stepping left/right leads off an edge => if so, stop.
        3) Move horizontally, apply collisions, then apply gravity/collision.
        """
        # Chase
        if player.rect.centerx < self.rect.centerx:
            self.vx = -ENEMY_SPEED
        else:
            self.vx = ENEMY_SPEED

        # Edge check
        next_x = self.rect.x + self.vx
        front_edge_x = next_x + (self.rect.width // 2) * (1 if self.vx>0 else -1)
        check_x = front_edge_x
        check_y = self.rect.bottom + 2
        if not is_solid_at(check_x, check_y, tile_sprites):
            self.vx = 0  # don't walk off

        # Move horizontally
        self.rect.x += self.vx
        self.check_collisions_x(tile_sprites)

        # Gravity
        self.vy += GRAVITY
        if self.vy > MAX_FALL_SPEED:
            self.vy = MAX_FALL_SPEED
        self.rect.y += self.vy
        self.check_collisions_y(tile_sprites)

    def check_collisions_x(self, tile_sprites):
        hits = [t for t in tile_sprites if t.solid and self.rect.colliderect(t.rect)]
        for t in hits:
            if self.vx > 0:
                self.rect.right = t.rect.left
            elif self.vx < 0:
                self.rect.left = t.rect.right
            self.vx = 0

    def check_collisions_y(self, tile_sprites):
        hits = [t for t in tile_sprites if t.solid and self.rect.colliderect(t.rect)]
        for t in hits:
            if self.vy > 0:
                self.rect.bottom = t.rect.top
            else:
                self.rect.top = t.rect.bottom
            self.vy = 0

###############################################################################
#                       PLAYER CLASS
###############################################################################
class Player(pygame.sprite.Sprite):
    """
    Our main character. M in the ASCII map indicates spawn.
    Collisions with enemies cost 1 HP; if HP < 0 => Game Over.
    """
    def __init__(self, x, y):
        super().__init__()
        self.image = player_surf
        self.rect = self.image.get_rect(topleft=(x,y))
        self.vx = 0
        self.vy = 0
        self.on_ground = False
        self.lives = PLAYER_START_LIVES
        self.invuln = False
        self.invuln_timer = 0
        self.score = 0

    def move(self, dx):
        self.vx = dx

    def jump(self):
        if self.on_ground:
            self.vy = -PLAYER_JUMP_SPEED
            self.on_ground = False

    def update(self, tile_sprites):
        # Gravity
        self.vy += GRAVITY
        if self.vy > MAX_FALL_SPEED:
            self.vy = MAX_FALL_SPEED

        # Horizontal
        self.rect.x += self.vx
        self.check_collisions_x(tile_sprites)

        # Vertical
        self.rect.y += self.vy
        self.on_ground = False
        self.check_collisions_y(tile_sprites)

        # Invulnerability cooldown
        if self.invuln:
            now = pygame.time.get_ticks()
            if now - self.invuln_timer > INVULN_TIME_MS:
                self.invuln = False

    def check_collisions_x(self, tile_sprites):
        hits = [t for t in tile_sprites if t.solid and self.rect.colliderect(t.rect)]
        for t in hits:
            if self.vx > 0:
                self.rect.right = t.rect.left
            elif self.vx < 0:
                self.rect.left = t.rect.right
            self.vx = 0

    def check_collisions_y(self, tile_sprites):
        hits = [t for t in tile_sprites if t.solid and self.rect.colliderect(t.rect)]
        for t in hits:
            if self.vy > 0:
                self.rect.bottom = t.rect.top
                self.on_ground = True
            elif self.vy < 0:
                self.rect.top = t.rect.bottom
            self.vy = 0

###############################################################################
#                     HELPER: IS SOLID AT (X, Y)?
###############################################################################
def is_solid_at(px, py, tile_sprites):
    for tile in tile_sprites:
        if tile.solid and tile.rect.collidepoint(px, py):
            return True
    return False

###############################################################################
#                     BUILD LEVEL FROM ASCII
###############################################################################
def build_level(level_data):
    """
    Convert the ASCII map into tile sprites, enemy sprites, and find the player spawn.
    M => player spawn
    G => enemy
    """
    tile_sprites = pygame.sprite.Group()
    enemy_sprites = pygame.sprite.Group()
    player = None

    for row_idx, row in enumerate(level_data):
        for col_idx, char in enumerate(row):
            x = col_idx * TILE_SIZE
            y = row_idx * TILE_SIZE

            if char in ['#','B','?','P','.']:
                tile = Tile(x, y, char)
                tile_sprites.add(tile)
            elif char == 'G':
                # place sky tile
                tile = Tile(x, y, '.')
                tile_sprites.add(tile)
                # create enemy
                goomba = Enemy(x, y)
                enemy_sprites.add(goomba)
            elif char == 'M':
                # place sky tile
                tile = Tile(x, y, '.')
                tile_sprites.add(tile)
                # create player
                player = Player(x, y)
            else:
                # default: sky
                tile = Tile(x, y, '.')
                tile_sprites.add(tile)

    return tile_sprites, enemy_sprites, player

###############################################################################
#                          DRAW BACKGROUND
###############################################################################
def draw_background(screen, camera_x):
    """
    Parallax scrolling with two layers: far (moves slower) and near (moves faster).
    """
    offset_far  = int(camera_x * 0.2) % SCREEN_WIDTH
    offset_near = int(camera_x * 0.5) % SCREEN_WIDTH

    # draw far
    screen.blit(bg_far, (-offset_far, 0))
    screen.blit(bg_far, (SCREEN_WIDTH - offset_far, 0))

    # draw near
    screen.blit(bg_near, (-offset_near, 0))
    screen.blit(bg_near, (SCREEN_WIDTH - offset_near, 0))

###############################################################################
#                     ENEMY COLLISION (NO KILL)
###############################################################################
def handle_enemy_collisions(player, enemy_sprites):
    """
    If the player collides with any enemy:
    - Lose 1 HP
    - Become invulnerable for 1s
    - The enemy remains (no kill).
    """
    hits = pygame.sprite.spritecollide(player, enemy_sprites, False)
    if not hits:
        return
    # If invulnerable, ignore
    if player.invuln:
        return

    # Take damage
    now = pygame.time.get_ticks()
    player.lives -= ENEMY_DAMAGE
    player.invuln = True
    player.invuln_timer = now

###############################################################################
#                           MAIN GAME LOOP
###############################################################################
def main():
    current_level_idx = 0
    total_levels = len(ALL_LEVELS)

    # Build first level
    tile_sprites, enemy_sprites, player = build_level(ALL_LEVELS[current_level_idx])
    game_state = STATE_PLAYING

    # Determine level width in pixels from row length
    level_width_in_pixels = len(ALL_LEVELS[current_level_idx][0]) * TILE_SIZE
    camera_x = 0

    while True:
        dt = clock.tick(FPS)

        # Events
        for event in pygame.event.get():
            if event.type == QUIT:
                pygame.quit()
                sys.exit()
            elif event.type == KEYDOWN:
                if event.key == K_ESCAPE:
                    pygame.quit()
                    sys.exit()
                if game_state == STATE_PLAYING:
                    if event.key == K_SPACE:
                        player.jump()
                else:
                    # if GAMEOVER or WIN, pressing SPACE/ENTER restarts or next
                    if event.key in (K_SPACE, K_RETURN):
                        if game_state == STATE_GAMEOVER:
                            current_level_idx = 0  # back to level 1
                        else:  # game_state == STATE_WIN
                            current_level_idx += 1
                            if current_level_idx >= total_levels:
                                current_level_idx = 0

                        tile_sprites, enemy_sprites, player = build_level(ALL_LEVELS[current_level_idx])
                        level_width_in_pixels = len(ALL_LEVELS[current_level_idx][0]) * TILE_SIZE
                        camera_x = 0
                        game_state = STATE_PLAYING

        # Movement (only if playing)
        keys = pygame.key.get_pressed()
        if game_state == STATE_PLAYING:
            if keys[K_LEFT]:
                player.move(-PLAYER_MOVE_SPEED)
            elif keys[K_RIGHT]:
                player.move(PLAYER_MOVE_SPEED)
            else:
                player.move(0)

        # Update
        if game_state == STATE_PLAYING:
            # Update player
            player.update(tile_sprites)

            # If fell off the stage => game over
            if player.rect.y > SCREEN_HEIGHT + 200:
                game_state = STATE_GAMEOVER

            # Update enemies
            for e in enemy_sprites:
                e.update(tile_sprites, player)

            # Handle collisions (no kill)
            handle_enemy_collisions(player, enemy_sprites)

            # Check HP
            if player.lives < 0:
                game_state = STATE_GAMEOVER

            # Check if we reached the right side => next level
            if player.rect.right > level_width_in_pixels - TILE_SIZE:
                game_state = STATE_WIN

            # Camera
            camera_x = player.rect.centerx - SCREEN_WIDTH // 2
            if camera_x < 0:
                camera_x = 0
            if camera_x > level_width_in_pixels - SCREEN_WIDTH:
                camera_x = level_width_in_pixels - SCREEN_WIDTH

        # Draw
        draw_background(screen, camera_x)

        # Tiles
        for t in tile_sprites:
            screen.blit(t.image, (t.rect.x - camera_x, t.rect.y))

        # Enemies
        for e in enemy_sprites:
            screen.blit(e.image, (e.rect.x - camera_x, e.rect.y))

        # Player (blink if invuln)
        if game_state == STATE_PLAYING and player.invuln:
            now = pygame.time.get_ticks()
            if (now // 100) % 2 == 0:
                screen.blit(player.image, (player.rect.x - camera_x, player.rect.y))
        else:
            screen.blit(player.image, (player.rect.x - camera_x, player.rect.y))

        # HUD
        font = pygame.font.SysFont(None, 24)
        lives_surf = font.render(f"Lives: {player.lives}", True, WHITE)
        screen.blit(lives_surf, (10,10))
        level_surf = font.render(f"Level: {current_level_idx+1}", True, WHITE)
        screen.blit(level_surf, (10,35))

        # Game Over / Win
        if game_state == STATE_GAMEOVER:
            go_txt = "GAME OVER! Press SPACE/ENTER to restart at Level 1."
            go_surf = font.render(go_txt, True, RED)
            screen.blit(go_surf, (SCREEN_WIDTH//2 - 200, SCREEN_HEIGHT//2))
        elif game_state == STATE_WIN:
            if current_level_idx < total_levels - 1:
                msg = "Level Complete! Press SPACE/ENTER for Next Level."
            else:
                msg = "You Beat All 5 Levels! Press SPACE/ENTER to Restart."
            win_surf = font.render(msg, True, GREEN)
            screen.blit(win_surf, (SCREEN_WIDTH//2 - 220, SCREEN_HEIGHT//2))

        pygame.display.flip()

###############################################################################
#                               RUN GAME
###############################################################################
if __name__ == "__main__":
    main()
