import math
import random
import sys

import pygame

# initialize
pygame.init()

# create the screen
screen = pygame.display.set_mode((800, 600))

# Background
background = pygame.image.load('background.png')

# title and icon
pygame.display.set_caption("Chicken Invaders")
icon = pygame.image.load('img.png')
pygame.display.set_icon(icon)

# Player
player_img = pygame.image.load('spaceship.png')
playerX = 370
playerY = 480
playerX_change = 0
playerY_change = 0

# Enemy chick
enemy_img_chick = pygame.image.load('chick.png')
# enemyX_chick = random.randint(0, 736)
# enemyY_chick = random.randint(50, 150)
enemyX_chick = []
enemyY_chick = []
enemyX_change_chick = []
enemyY_change_chick = []
num_of_chicks = 10

x = 70
for i in range(num_of_chicks):
    enemyX_chick.append(x)
    enemyY_chick.append(120)
    enemyX_change_chick.append(0.2)
    enemyY_change_chick.append(70)
    x += 70

# Enemy chicken
enemy_img_chicken = pygame.image.load('chicken.png')
# enemyX_chick = random.randint(0, 736)
# enemyY_chick = random.randint(50, 150)
enemyX_chicken = []
enemyY_chicken = []
enemyX_change_chicken = []
enemyY_change_chicken = []
num_of_chickens = 10

x = 70
for i in range(num_of_chickens):
    enemyX_chicken.append(x)
    enemyY_chicken.append(50)
    enemyX_change_chicken.append(-0.2)
    enemyY_change_chicken.append(70)
    x += 70

# Bullet
bullet_img = pygame.image.load('bullet.png')
bulletX = 0
bulletY = 0
bulletY_change = 0.5
meci = []

# Egg
egg_img = pygame.image.load('egg.png')
eggX = 0
eggY = 0
eggY_change = 0.4
eggs = []
br = 0

# Roasted chicken
roasted_img = pygame.image.load('roasted_chicken.png')
roastedX = 0
roastedY = 0
roastedY_change = 0.2
roasted = []

# Roasted chick
roasted_chick_img = pygame.image.load('roasted_chick.png')
roasted_chick_X = 0
roasted_chick_Y = 0
roasted_chick_Y_change = 0.2
roasted_chicks = []

# Score
score_value = 0
font = pygame.font.Font("freesansbold.ttf", 32)
textX = 10
texty = 10

# Life
life_value = 3
lifeX = 650
lifeY = 10

# Game over
game_over_font = pygame.font.Font("freesansbold.ttf", 64)

# Victory
victory_font = pygame.font.Font("freesansbold.ttf", 64)


def show_score(x, y):
    score = font.render("Score : " + str(score_value), True, (255, 255, 255))
    screen.blit(score, (x, y))


def show_life(x, y):
    life = font.render("Life : " + str(life_value), True, (255, 255, 255))
    screen.blit(life, (x, y))


def game_over():
    game_over_text = game_over_font.render("GAME OVER", True, (255, 255, 255))
    screen.blit(game_over_text, (200, 250))


def victory():
    victory_text = game_over_font.render("VICTORY!", True, (255, 255, 255))
    screen.blit(victory_text, (250, 250))


def main_menu():
    main_menu_text = game_over_font.render("MAIN MENU", True, (255, 255, 255))
    screen.blit(main_menu_text, (200, 200))


def player(x, y):
    screen.blit(player_img, (x, y))


def enemy_chick(x, y):
    screen.blit(enemy_img_chick, (x, y))


def enemy_chicken(x, y):
    screen.blit(enemy_img_chicken, (x, y))


def fire_bullet(x, y):
    screen.blit(bullet_img, (x + 16, y + 10))


def egg(x, y):
    screen.blit(egg_img, (x + 8, y + 5))


def roasted_chicken(x, y):
    screen.blit(roasted_img, (x + 8, y + 5))


def roasted_chick(x, y):
    screen.blit(roasted_chick_img, (x + 8, y + 5))


def isCollision(enemyX, enemyY, bullets):
    for brojac in range(len(bullets)):
        x = bullets[brojac][0]
        y = bullets[brojac][1]
        distance = math.sqrt(math.pow(enemyX - x, 2) + (math.pow(enemyY - y, 2)))
        if distance < 27:
            return [x, y]
    return None


# menu loop
click = False
running = True
while running:
    # Background
    screen.blit(background, (0, 0))
    main_menu()

    mx, my = pygame.mouse.get_pos()

    button = pygame.Rect(300, 350, 200, 75)
    start = font.render("START", True, (255, 255, 255))

    pygame.draw.rect(screen, (0, 0, 70), button)
    screen.blit(start, (350, 370))

    if button.collidepoint((mx, my)):
        pygame.draw.rect(screen, (100, 0, 100), button)
        screen.blit(start, (350, 370))
        if click:
            running = False

    click = False
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
        if event.type == pygame.MOUSEBUTTONDOWN:
            click = True

    pygame.display.update()

# game loop
running = True
while running:

    # RGB
    screen.fill((0, 0, 50))
    # Background
    screen.blit(background, (0, 0))

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        # provjeri je li pritisnuta tipka, ako je provjeri koja
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:
                playerX_change = -0.3
            if event.key == pygame.K_RIGHT:
                playerX_change = 0.3
            if event.key == pygame.K_UP:
                playerY_change = -0.3
            if event.key == pygame.K_DOWN:
                playerY_change = 0.3

            if event.key == pygame.K_SPACE:
                bulletX = playerX
                bulletY = playerY
                meci.append([bulletX, bulletY])

        if event.type == pygame.KEYUP:
            if event.key == pygame.K_LEFT or event.key == pygame.K_RIGHT:
                playerX_change = 0
            if event.key == pygame.K_UP or event.key == pygame.K_DOWN:
                playerY_change = 0

    # checking player boundaries
    playerX += playerX_change
    if playerX <= 0:
        playerX = 0
    if playerX >= 736:
        playerX = 736

    playerY += playerY_change
    if playerY <= 0:
        playerY = 0
    if playerY >= 536:
        playerY = 536

    # checking enemy movement
    # chick
    for i in range(len(enemyX_chick)):
        enemyX_chick[i] += enemyX_change_chick[i]
        if enemyX_chick[i] <= 0:
            enemyX_change_chick[i] = 0.2
            enemyY_chick[i] += enemyY_change_chick[i]
        if enemyX_chick[i] >= 736:
            enemyX_change_chick[i] = -0.2
            enemyY_chick[i] += enemyY_change_chick[i]
    # chicken
    for i in range(len(enemyX_chicken)):
        enemyX_chicken[i] += enemyX_change_chicken[i]
        if enemyX_chicken[i] <= 0:
            enemyX_change_chicken[i] = 0.2
            enemyY_chicken[i] += enemyY_change_chicken[i]
        if enemyX_chicken[i] >= 736:
            enemyX_change_chicken[i] = -0.2
            enemyY_chicken[i] += enemyY_change_chicken[i]

    # bullet movement
    if meci:
        brojac = 0
        bullets = meci.copy()
        for brojac in range(len(bullets)):
            x = bullets[brojac][0]
            y = bullets[brojac][1]
            fire_bullet(x, y)
            if y <= 0:
                meci.remove([x, y])
            else:
                i = meci.index([x, y])
                y -= bulletY_change
                meci[i] = [x, y]

    kopijaX = enemyX_chick.copy()
    kopijaY = enemyY_chick.copy()
    for i in range(len(kopijaX)):
        # Collision enemy chick - bullet
        collision = isCollision(kopijaX[i], kopijaY[i], meci)
        if collision is not None:
            x = collision[0]
            y = collision[1]
            meci.remove([x, y])

            roasted_chick_X = enemyX_chick[i]
            roasted_chick_Y = enemyY_chick[i]
            roasted_chicks.append([roasted_chick_X, roasted_chick_Y])

            enemyX_chick.remove(enemyX_chick[i])
            enemyY_chick.remove(enemyY_chick[i])
            enemyX_change_chick.remove(enemyX_change_chick[i])
            enemyY_change_chick.remove(enemyY_change_chick[i])

        # provjera sudara broda i pilica
        distance = math.sqrt(math.pow(kopijaX[i] - playerX, 2) + (math.pow(kopijaY[i] - playerY, 2)))
        if distance < 27:
            life_value -= 1

            enemyX_chick.remove(enemyX_chick[i])
            enemyY_chick.remove(enemyY_chick[i])
            enemyX_change_chick.remove(enemyX_change_chick[i])
            enemyY_change_chick.remove(enemyY_change_chick[i])

    kopijaX = enemyX_chicken.copy()
    kopijaY = enemyY_chicken.copy()
    for i in range(len(kopijaX)):
        # Collision enemy chicken - bullet
        collision = isCollision(kopijaX[i], kopijaY[i], meci)
        if collision is not None:
            x = collision[0]
            y = collision[1]
            meci.remove([x, y])

            roastedX = enemyX_chicken[i]
            roastedY = enemyY_chicken[i]
            roasted.append([roastedX, roastedY])

            enemyX_chicken.remove(enemyX_chicken[i])
            enemyY_chicken.remove(enemyY_chicken[i])
            enemyX_change_chicken.remove(enemyX_change_chicken[i])
            enemyY_change_chicken.remove(enemyY_change_chicken[i])

        # provjera sudara broda i kokosi
        distance = math.sqrt(math.pow(kopijaX[i] - playerX, 2) + (math.pow(kopijaY[i] - playerY, 2)))
        if distance < 27:
            life_value -= 1

            enemyX_chicken.remove(enemyX_chicken[i])
            enemyY_chicken.remove(enemyY_chicken[i])
            enemyX_change_chicken.remove(enemyX_change_chicken[i])
            enemyY_change_chicken.remove(enemyY_change_chicken[i])

    # egg spawn
    if enemyX_chicken:
        if br == 2500:
            eggX = random.choice(enemyX_chicken)
            eggY = random.choice(enemyY_chicken)
            eggs.append([eggX, eggY])
            eggX = random.choice(enemyX_chicken)
            eggY = random.choice(enemyY_chicken)
            eggs.append([eggX, eggY])
            br = 0
        br += 1

    # egg movement
    if eggs:
        jaja = eggs.copy()
        for brojac in range(len(jaja)):
            x = jaja[brojac][0]
            y = jaja[brojac][1]
            egg(x, y)
            if y >= 545:
                eggs.remove([x, y])
            else:
                i = eggs.index([x, y])
                y += eggY_change
                eggs[i] = [x, y]
        # Collision player - egg
        collision = isCollision(playerX, playerY, eggs)
        if collision is not None:
            life_value -= 1
            x = collision[0]
            y = collision[1]
            eggs.remove([x, y])

    # roasted movement
    if roasted:
        kokos = roasted.copy()
        for brojac in range(len(kokos)):
            x = kokos[brojac][0]
            y = kokos[brojac][1]
            roasted_chicken(x, y)
            if y >= 545:
                roasted.remove([x, y])
            else:
                i = roasted.index([x, y])
                y += roastedY_change
                roasted[i] = [x, y]

    # Collision player - food
    collision = isCollision(playerX, playerY, roasted)
    if collision is not None:
        x = collision[0]
        y = collision[1]
        roasted.remove([x, y])
        score_value += 3

    # roasted chick movement
    if roasted_chicks:
        pilici = roasted_chicks.copy()
        for brojac in range(len(pilici)):
            x = pilici[brojac][0]
            y = pilici[brojac][1]
            roasted_chick(x, y)
            if y >= 545:
                roasted_chicks.remove([x, y])
            else:
                i = roasted_chicks.index([x, y])
                y += roasted_chick_Y_change
                roasted_chicks[i] = [x, y]

    # Collision player - food
    collision = isCollision(playerX, playerY, roasted_chicks)
    if collision is not None:
        x = collision[0]
        y = collision[1]
        roasted_chicks.remove([x, y])
        score_value += 1

    # provjera je li kraj
    if (life_value == 0):
        for j in range(len(enemyX_chicken)):
            enemyY_chicken[j] = 2000
        for j in range(len(enemyX_chick)):
            enemyY_chick[j] = 2000
        game_over()

    # provjera je li pobjeda
    if not enemyX_chicken and not enemyX_chick:
        victory()

    # prikazi komponente
    for i in range(len(enemyX_chick)):
        enemy_chick(enemyX_chick[i], enemyY_chick[i])
    for i in range(len(enemyX_chicken)):
        enemy_chicken(enemyX_chicken[i], enemyY_chicken[i])

    player(playerX, playerY)
    show_score(textX, texty)
    show_life(lifeX, lifeY)
    pygame.display.update()
