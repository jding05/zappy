import socket
import sys
import select
import pygame
from pygame.locals import *
from grid import *
from player import *

buf_sz = 8192

tile_sz = 120
item_sz = 25
player_sz = 60

food = 0
linemate = 1
deraumere = 2
sibur = 3
mendiane = 4
phiras = 5
thystame = 6

def load_source():

    tile = pygame.transform.scale(pygame.image.load('./textures/tile.jpg').convert_alpha(), (tile_sz, tile_sz))
    items = {
        food: pygame.transform.scale(pygame.image.load('./textures/item/food.png').convert_alpha(), (item_sz, item_sz)),
        linemate: pygame.transform.scale(pygame.image.load('./textures/item/linemate54.png').convert_alpha(), (item_sz, item_sz)),
        deraumere: pygame.transform.scale(pygame.image.load('./textures/item/deraumere54.png').convert_alpha(), (item_sz, item_sz)),
        sibur: pygame.transform.scale(pygame.image.load('./textures/item/sibur54.png').convert_alpha(), (item_sz, item_sz)),
        mendiane: pygame.transform.scale(pygame.image.load('./textures/item/mendiane54.png').convert_alpha(), (item_sz, item_sz)),
        phiras: pygame.transform.scale(pygame.image.load('./textures/item/phiras54.png').convert_alpha(), (item_sz, item_sz)),
        thystame: pygame.transform.scale(pygame.image.load('./textures/item/thystame54.png').convert_alpha(), (item_sz, item_sz))
    }
    dead = pygame.transform.scale(pygame.image.load('./sprites/dead.png').convert_alpha(), (45, 60))
    return tile, items, dead

def start_game():

    pygame.init()
    pygame.display.set_caption('[Zappy]    gfx client')
    window = pygame.display.set_mode((1200, 923), DOUBLEBUF)
    intro = pygame.image.load('./textures/zappy.png').convert_alpha()
    window.blit(intro, (0, 0))
    pygame.display.flip()
    start = False
    while not start:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                sys.exit(0)
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_KP_ENTER or event.key == pygame.K_RETURN:
                    start = True
                    pygame.display.quit()
                elif event.key == pygame.K_ESCAPE or event.key == pygame.K_q:
                    sys.exit(0)

def connect_init():

    host_ip = '127.0.0.1'
    port = 4242
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((host_ip, port))
    data = "gfx"
    for x in range(buf_sz - len("gfx")):
        data += '#'
    s.send(data)
    data = s.recv(buf_sz).split("@")[0].split(",")
    col = int(data[0])
    row = int(data[1])
    return col, row, s

def main():

    start_game()
    col, row, s = connect_init()
    window = pygame.display.set_mode((col * tile_sz, row * tile_sz), DOUBLEBUF)
    tile, items, dead = load_source()
    for r in range(row):
        for c in range(col):
            window.blit(tile, (c * tile_sz, r * tile_sz))
    pygame.display.flip()
    ALL_PLAYER = {}
    grids = []
    old_data = ''
    while True:
        readable, writable, exceptional = select.select([s], [1], [], 0.1)
        if s in readable:
            data = ''
            GET_FULL_DATA = False
            while not GET_FULL_DATA:
                data += s.recv(buf_sz).replace("#", "")
                GET_FULL_DATA = "@" in data
            if data == old_data or data == '':
                continue
            data_split = data.split("\n")
            print(data_split)
            map_data = data_split.pop(0).split(",")
            if not grids:
                for r in range(row):
                    grids.append([])
                    for c in range(col):
                        new_grid = Grid()
                        new_grid.setup(tile, int(map_data.pop(0)), [])
                        grids[r].append(new_grid)
            else:
                for r in range(row):
                    for c in range(col):
                        grids[r][c].updateitem(int(map_data.pop(0)))
            while data_split[0] != '@' and data_split[0] != '':
                new_p = Player()
                new_p.setup(data_split.pop(0))
                if new_p.id in ALL_PLAYER and new_p.dead != 1 and new_p.left != 1:
                    grids[ALL_PLAYER[new_p.id].coor[1]][ALL_PLAYER[new_p.id].coor[0]].removeplayer(new_p.id)
                    ALL_PLAYER[new_p.id].coor = new_p.coor
                    grids[new_p.coor[1]][new_p.coor[0]].addplayer(new_p)
                elif new_p.id in ALL_PLAYER and new_p.dead == 1:
                    grids[new_p.coor[1]][new_p.coor[0]].updateplayer(new_p.id, 0, 0, dead)
                elif new_p.id in ALL_PLAYER and new_p.left == 1:
                    grids[new_p.coor[1]][new_p.coor[0]].removeplayer(new_p.id)
                else:
                    ALL_PLAYER[new_p.id] = new_p
                    grids[new_p.coor[1]][new_p.coor[0]].addplayer(new_p)
            for r in range(row):
                for c in range(col):
                    window.blit(grids[r][c].background, (c * tile_sz, r * tile_sz))
                    for x in range(7):
                        if grids[r][c].items[x][2] is 1:
                            window.blit(items[x], (c * tile_sz + (tile_sz - item_sz) * grids[r][c].items[x][0], r * tile_sz + (tile_sz - item_sz) * grids[r][c].items[x][1]))
            for r in range(row):
                for c in range(col):
                    for p in range(len(grids[r][c].players)):
                        xcoor = int(c * tile_sz + (tile_sz - player_sz) * grids[r][c].players[p][0] +
                                    grids[r][c].players[p][2].xshift * tile_sz)
                        ycoor = int(r * tile_sz + (tile_sz - player_sz) * grids[r][c].players[p][1] +
                                    grids[r][c].players[p][2].yshift * tile_sz)
                        window.blit((grids[r][c].players[p][2]).img, (xcoor, ycoor))
            old_data = data
        pygame.display.flip()
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                sys.exit(0)
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    sys.exit(0)
if __name__ == '__main__':
    main()
