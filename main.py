import pygame
#import initialExample as ecosim
import World as ecosim

pygame.init()
sim = ecosim.World()
WIDTH, HEIGHT = sim.get_width(), sim.get_height()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Ecosystem")
clock = pygame.time.Clock()

# 顏色定義：0: 植物(綠), 1: 草食(藍), 2: 肉食(紅)
COLORS = {
    0: (46, 204, 113),
    1: (52, 152, 219),
    2: (231, 76, 60),
    3: (155, 89, 182)
}

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # 1. 叫 C++ 計算下一幀
    sim.step()

    # 2. 向 C++ 要這一幀的繪圖資料
    entities = sim.get_render_entities()

    # 3. 畫面更新
    screen.fill((30, 30, 30))
    for e in entities:
        pygame.draw.circle(screen, COLORS[e.type], (int(e.x), int(e.y)), int(e.radius))

    pygame.display.flip()
    clock.tick(60)

pygame.quit()