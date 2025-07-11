import pygame, sys, time, math

WIDTH = 1000
HEIGHT = 800
SCALE = 2

TILE_SIZE = 12
CHUNK_SIZE = 8

class Editor:
    def __init__(self):
        self.screen = pygame.Surface((WIDTH // SCALE, HEIGHT // SCALE))
        self.display = pygame.display.set_mode((WIDTH, HEIGHT), flags=pygame.RESIZABLE)
        self.clock = pygame.time.Clock()

        self.dt = 1
        self.last_time = time.time() - 1/60

        self.running = True
        self.controls = {'left': False, 'right': False, 'up': False, 'down': False}

        self.scroll_speed = 5
        self.scroll = pygame.Vector2(0, 0)
    
    def close():
        pygame.quit()
        sys.exit()

    def update(self):

        self.scroll.x += (self.controls['right'] - self.controls['left']) * self.scroll_speed * self.dt
        self.scroll.y += (self.controls['down'] - self.controls['up']) * self.scroll_speed * self.dt

        render_scroll = [int(self.scroll.x), int(self.scroll.y)]

        self.screen.fill((0, 0, 0))
        self.draw_tile_grid(render_scroll, [1, 1], (100, 100, 100))
        self.draw_tile_grid(render_scroll, [CHUNK_SIZE, CHUNK_SIZE], (200, 100, 150))
        pygame.draw.rect(self.screen, (255, 0, 0), (50 - render_scroll[0], 50 - render_scroll[1], 20, 20))


    def draw_tile_grid(self, scroll, size: list | tuple, color):
        tile_size = [TILE_SIZE * size[0], TILE_SIZE * size[1]]
        length = math.ceil(self.screen.get_width() / tile_size[0]) + 2
        height = math.ceil(self.screen.get_height() / tile_size[1]) + 2
        for x in range(length):
            pygame.draw.line(self.screen, color, ((x - 1) * tile_size[0] - (scroll[0] % tile_size[0]), 0), ((x - 1) * tile_size[0] - (scroll[0] % tile_size[0]), self.screen.get_height()))
        for y in range(height):
            pygame.draw.line(self.screen, color, (0, (y - 1) * tile_size[1] - (scroll[1] % tile_size[1])), (self.screen.get_width(), (y - 1) * tile_size[1] - (scroll[1] % tile_size[1])))

    def run(self):
        while self.running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.running = False
                    self.close()
                if event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_ESCAPE:
                        self.running = False
                        self.close()
                    if event.key == pygame.K_LEFT:
                        self.controls['left'] = True
                    if event.key == pygame.K_RIGHT:
                        self.controls['right'] = True
                    if event.key == pygame.K_UP:
                        self.controls['up'] = True
                    if event.key == pygame.K_DOWN:
                        self.controls['down'] = True
                if event.type == pygame.KEYUP:
                    if event.key == pygame.K_LEFT:
                        self.controls['left'] = False
                    if event.key == pygame.K_RIGHT:
                        self.controls['right'] = False
                    if event.key == pygame.K_UP:
                        self.controls['up'] = False
                    if event.key == pygame.K_DOWN:
                        self.controls['down'] = False
                if event.type == pygame.WINDOWRESIZED:
                    self.screen = pygame.Surface((self.display.get_width() / SCALE, self.display.get_height() / SCALE))

            self.dt = time.time() - self.last_time
            self.dt *= 60
            self.last_time = time.time()
            self.update()

            self.display.blit(pygame.transform.scale(self.screen, self.display.get_size()), (0, 0))
            pygame.display.set_caption(f"FPS: {self.clock.get_fps() :.1f}")
            pygame.display.flip()

            self.clock.tick()

Editor().run()