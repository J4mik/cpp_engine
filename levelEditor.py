import pygame, sys, time

WIDTH = 1000
HEIGHT = 800
SCALE = 2

class Editor:
    def __init__(self):
        self.screen = pygame.Surface((WIDTH // SCALE, HEIGHT // SCALE))
        self.display = pygame.display.set_mode((WIDTH, HEIGHT))
        self.clock = pygame.time.Clock()

        self.dt = 1
        self.last_time = time.time() - 1/60

        self.running = True
    
    def close():
        pygame.quit()
        sys.exit()

    def update(self):
        self.screen.fill((100, 60, 0))

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

            self.dt = time.time() - self.last_time
            self.dt *= 60
            self.last_time = time.time()
            self.update()

            pygame.transform.scale_by(self.screen, SCALE, self.display)
            pygame.display.set_caption(f"FPS: {self.clock.get_fps() :.1f}")
            pygame.display.flip()

            self.clock.tick()

Editor().run()