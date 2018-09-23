# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    python_client.py                                   :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sding <sding@student.42.us.org>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/09/17 22:37:33 by sding             #+#    #+#              #
#    Updated: 2018/09/17 22:37:34 by sding            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import pygame
import socket
import select
import sys
from os import system, name
import time

TCP_IP = '127.0.0.1'
TCP_PORT = 4242
TEAM_NAME = ""
TEAM_NAME_MSG = ""
BUFFER_SIZE = 8192
s = 0
start = 0

def usage ():

    print ("Usage: ./client -n <team> -p <port> [-h <hostname>]")
    print ("\t-n team\_name")
    print ("\t-p port")
    print ("\t-h name of the host, by default it'll be localhost")
    sys.exit(1)

def connect_game_server ():

    global TCP_IP
    global TCP_PORT
    global TEAM_NAME
    global BUFFER_SIZE
    global s

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((TCP_IP, TCP_PORT))
    # to send the team_name to server to verify
    for x in range(BUFFER_SIZE - len(TEAM_NAME)):
        TEAM_NAME += '#'
    s.send(TEAM_NAME)
    # if team_name match, then the server send back welcome function
    data = s.recv(BUFFER_SIZE)
    welcome_msg = data.replace("#", "")
    print ("received data:" + welcome_msg)
    # server send back how many more client can you connect
    data = s.recv(BUFFER_SIZE)
    connect_nbr = data.replace("#", "")
    print ("received data:" + connect_nbr)
    # the total map size of the server map
    data = s.recv(BUFFER_SIZE)
    map_size = data.replace("#", "")
    print ("received data:\n" + map_size)

def start_game_page (screen):

    global start

    for event in pygame.event.get():
        keys = pygame.key.get_pressed()
        if event.type == pygame.QUIT:
            sys.exit(0)
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_KP_ENTER or event.key == pygame.K_RETURN:
                start += 1
                connect_game_server()
                picture = pygame.transform.scale(pygame.image.load('./image/usage/usage_white_word_color.png'), (640, 480))
                screen.blit(picture, (0, 0))  # 100, 50 -> is the starty point (x, y) from the top left (0, 0)
            elif event.key == pygame.K_ESCAPE or event.key == pygame.K_q:
                sys.exit(0)

def msg_padding (str):

    for x in range(BUFFER_SIZE - len(str)):
        str += '#'
    return str

def main ():

    global TCP_IP
    global TCP_PORT
    global TEAM_NAME
    global BUFFER_SIZE
    global s
    global start

    ARGC = len(sys.argv)
    if ARGC < 5:
        usage()
    elif ARGC > 7:
        usage()
    elif ARGC == 5 or ARGC == 7:
        if ARGC == 7:
            if sys.argv[5] != "-h":
                usage()
            else:
                TCP_IP = sys.argv[6]
        if sys.argv[1] != "-n" or sys.argv[3] != "-p":
            usage()
        else:
            TEAM_NAME = sys.argv[2]
            TEAM_NAME_MSG = sys.argv[2]
            # print ("[" + TEAM_NAME + "]")
            if TEAM_NAME == "":
                usage()
            if sys.argv[4].isdigit() == True:
                TCP_PORT = int(sys.argv[4])
            else:
                usage()
    else:
        usage()

    # pygame pop window init and title
    pygame.init()
    pygame.display.set_caption('[Zappy]    player controller')

    # set the size of the window, and show the starting page
    screen = pygame.display.set_mode((640, 480))
    picture = pygame.transform.scale(pygame.image.load('./image/playtest.png'), (640, 480))
    screen.blit(picture, (0,0)) # 100, 50 -> is the starty point (x, y) from the top left (0, 0)
    pygame.display.update()


    system('clear')

    OVER = False
    start = 0

    while not OVER:

        """
         readable -> one is server, the other one is stdin (0)
         writable -> using pygame.event.get() by pressing the keybroad
         """
        if start == 1:
            readable, writable, exceptional = select.select([0, s], [1], [], 0.0)
            for i in readable:
                if i == s:
                    data = s.recv(BUFFER_SIZE)
                    if not data:
                        sys.exit(1)
                    else:
                        p = data.replace("#", "")
                        print ("received data:" + p)
                elif i == 0:
                    msg = raw_input()
                    array = msg.split(' ')
                    if len(array) >= 2:
                        if not cmp(array[0], "broadcast"):
                            for x in range(BUFFER_SIZE - len(msg)):
                                msg += '#'
                            s.send(msg)
                        else:
                            print ("invalid command")
                    else:
                        print ("invalid command")
            for i in writable:
                for event in pygame.event.get():
                    keys = pygame.key.get_pressed()
                    if event.type == pygame.QUIT:
                        # OVER = True
                        sys.exit(0)
                    elif event.type == pygame.KEYDOWN:
                        if event.key == pygame.K_ESCAPE:
                            # OVER = True
                            sys.exit(0)
                        elif event.key == pygame.K_UP:
                            data = msg_padding("advance")
                        elif event.key == pygame.K_RIGHT:
                            data = msg_padding("right")
                        elif event.key == pygame.K_LEFT:
                            data = msg_padding("left")
                        elif event.key == pygame.K_DOWN or event.key == pygame.K_s:
                            data = msg_padding ("see")
                        elif event.key == pygame.K_KP_PERIOD or event.key == pygame.K_i:
                            data = msg_padding ("inventory")
                        ##########################################################
                                    # take #
                        elif event.key == pygame.K_KP0:
                            data = msg_padding ("take food")
                        elif event.key == pygame.K_KP1:
                            data = msg_padding ("take linemate")
                        elif event.key == pygame.K_KP2:
                            data = msg_padding ("take deraumere")
                        elif event.key == pygame.K_KP3:
                            data = msg_padding ("take sibur")
                        elif event.key == pygame.K_KP4:
                            data = msg_padding ("take mendiane")
                        elif event.key == pygame.K_KP5:
                            data = msg_padding ("take phiras")
                        elif event.key == pygame.K_KP6:
                            data = msg_padding ("take thystame")
                        ##########################################################
                                    # put #
                        elif event.key == pygame.K_1:
                            data = msg_padding ("put linemate")
                        elif event.key == pygame.K_2:
                            data = msg_padding ("put deraumere")
                        elif event.key == pygame.K_3:
                            data = msg_padding ("put sibur")
                        elif event.key == pygame.K_4:
                            data = msg_padding ("put mendiane")
                        elif event.key == pygame.K_5:
                            data = msg_padding ("put phiras")
                        elif event.key == pygame.K_6:
                            data = msg_padding ("put thystame")
                        ##########################################################
                        elif event.key == pygame.K_k:
                            data = msg_padding ("kick")
                        ###########################################################
                             # cheating -> kick and advance
                        elif event.key == pygame.K_HOME:
                            cheating_data1 = msg_padding ("kick")
                            s.send(cheating_data1)

                            cheating_data2 = msg_padding ("advance")
                            s.send(cheating_data2)

                            cheating_data3 = msg_padding ("take food")
                            s.send(cheating_data3)

                            cheating_data4 = msg_padding ("take linemate")
                            s.send(cheating_data4)

                            cheating_data5 = msg_padding ("take deraumere")
                            s.send(cheating_data5)

                            cheating_data6 = msg_padding ("take sibur")
                            s.send(cheating_data6)

                            cheating_data7 = msg_padding ("take mendiane")
                            s.send(cheating_data7)

                            cheating_data8 = msg_padding ("take phiras")
                            s.send(cheating_data8)

                            data = msg_padding ("take thystame")
                        ###########################################################
                        elif event.key == pygame.K_d:
                            data = msg_padding ("incantation")
                        elif event.key == pygame.K_f:
                            data = msg_padding ("fork")
                        elif event.key == pygame.K_SPACE:
                            data = msg_padding ("connect_nbr")
                        elif event.key == pygame.K_RSHIFT:
                            picture = pygame.image.load('./image/level_requirement.png')
                            screen.fill((0,0,0))
                            screen.blit(picture, (0, 91))
                            data = msg_padding ("connect_nbr")
                        elif event.key == pygame.K_LSHIFT:
                            picture = pygame.transform.scale(pygame.image.load('./image/usage/usage_white_word_color.png'), (640, 480))
                            screen.blit(picture, (0, 0))
                            data = msg_padding ("connect_nbr")
                        else:
                            data = msg_padding ("broadcast I am here")
                        s.send(data)
        else:
            start_game_page(screen)
        pygame.display.update()


if __name__ == '__main__':
    main()
