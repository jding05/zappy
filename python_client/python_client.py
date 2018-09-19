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
BUFFER_SIZE = 8192
s = 0

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

    # get the full screen


    # time.sleep(1)

    # print (TEAM_NAME)
    # TEAM_NAME = "one"   ##############
    for x in range(BUFFER_SIZE - len(TEAM_NAME)):
        TEAM_NAME += '#'
    s.send(TEAM_NAME)

    data = s.recv(BUFFER_SIZE)
    welcome_msg = data.replace("#", "")
    print ("received data:" + welcome_msg)

    data = s.recv(BUFFER_SIZE)
    connect_nbr = data.replace("#", "")
    print ("received data:" + connect_nbr)

    data = s.recv(BUFFER_SIZE)
    map_size = data.replace("#", "")
    print ("received data:\n" + map_size)

def main ():

    # TCP_IP = '127.0.0.1'
    # TCP_PORT = 4242
    # TEAM_NAME = ""

    global TCP_IP
    global TCP_PORT
    global TEAM_NAME
    global BUFFER_SIZE
    global s

    ARGC = len(sys.argv)
    if ARGC < 5:
        usage()
    elif ARGC > 7:
        usage()
    elif ARGC == 5 or ARGC == 7:
        # parse(ARGC)
        if ARGC == 7:
            if sys.argv[5] != "-h":
                usage()
            else:
                TCP_IP = sys.argv[6]
        if sys.argv[1] != "-n" or sys.argv[3] != "-p":
            usage()
        else:
            TEAM_NAME = sys.argv[2]
            print ("[" + TEAM_NAME + "]")
            if TEAM_NAME == "":
                usage()
            if sys.argv[4].isdigit() == True:
                TCP_PORT = int(sys.argv[4])
            else:
                usage()
    else:
        usage()


    BUFFER_SIZE = 8192

    pygame.init()
    pygame.display.set_caption('[Zappy]    player controller')

    screen = pygame.display.set_mode((500, 300))

    OVER = False


    picture = pygame.transform.scale(pygame.image.load('./game_start.png'), (300, 200))
    screen.blit(picture, (100,50)) # 100, 50 -> is the starty point (x, y) from the top left (0, 0)
    pygame.display.update()

    system('clear')


    start = 0

    while not OVER:

    # '''
    #     readable -> one is server, the other one is stdin (0)
    #     writable -> using pygame.event.get() by pressing the keybroad
    # '''
        if start == 1:
            # print("here")
            readable, writable, exceptional = select.select([0, s], [1], [], 0.1)
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
                # if not i == s:
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
                            data = "advance"
                            for x in range(BUFFER_SIZE - len("advance")):
                                data += '#'
                        elif event.key == pygame.K_RIGHT:
                            data = "right"
                            for x in range(BUFFER_SIZE - len("right")):
                                data += '#'
                        elif event.key == pygame.K_LEFT:
                            data = "left"
                            for x in range(BUFFER_SIZE - len("left")):
                                data += '#'
                        elif event.key == pygame.K_DOWN or event.key == pygame.K_s:
                            data = "see"
                            for x in range(BUFFER_SIZE - len("see")):
                                data += '#'
                        elif event.key == pygame.K_KP_PERIOD or event.key == pygame.K_i:
                            data = "inventory"
                            for x in range(BUFFER_SIZE - len("inventory")):
                                data += '#'
                        ##########################################################
                                    # take #
                        elif event.key == pygame.K_KP0:
                            data = "take food"
                            for x in range(BUFFER_SIZE - len("take food")):
                                data += '#'
                        elif event.key == pygame.K_KP1:
                            data = "take linemate"
                            for x in range(BUFFER_SIZE - len("take linemate")):
                                data += '#'
                        elif event.key == pygame.K_KP2:
                            data = "take deraumere"
                            for x in range(BUFFER_SIZE - len("take deraumere")):
                                data += '#'
                        elif event.key == pygame.K_KP3:
                            data = "take sibur"
                            for x in range(BUFFER_SIZE - len("take sibur")):
                                data += '#'
                        elif event.key == pygame.K_KP4:
                            data = "take mendiane"
                            for x in range(BUFFER_SIZE - len("take mendiane")):
                                data += '#'
                        elif event.key == pygame.K_KP5:
                            data = "take phiras"
                            for x in range(BUFFER_SIZE - len("take phiras")):
                                data += '#'
                        elif event.key == pygame.K_KP6:
                            data = "take thystame"
                            for x in range(BUFFER_SIZE - len("take thystame")):
                                data += '#'
                        ##########################################################
                                    # put #
                        elif event.key == pygame.K_1:
                            data = "put linemate"
                            for x in range(BUFFER_SIZE - len("put linemate")):
                                data += '#'
                        elif event.key == pygame.K_2:
                            data = "put deraumere"
                            for x in range(BUFFER_SIZE - len("put deraumere")):
                                data += '#'
                        elif event.key == pygame.K_3:
                            data = "put sibur"
                            for x in range(BUFFER_SIZE - len("put sibur")):
                                data += '#'
                        elif event.key == pygame.K_4:
                            data = "put mendiane"
                            for x in range(BUFFER_SIZE - len("put mendiane")):
                                data += '#'
                        elif event.key == pygame.K_5:
                            data = "put phiras"
                            for x in range(BUFFER_SIZE - len("put phiras")):
                                data += '#'
                        elif event.key == pygame.K_6:
                            data = "put thystame"
                            for x in range(BUFFER_SIZE - len("put thystame")):
                                data += '#'
                        ##########################################################
                        elif event.key == pygame.K_k:
                            data = "kick"
                            for x in range(BUFFER_SIZE - len("kick")):
                                data += '#'
                        ###########################################################
                             # cheating -> kick and advance
                        elif event.key == pygame.K_a:
                            cheating_data1 = "kick"
                            for x in range(BUFFER_SIZE - len("kick")):
                                cheating_data1 += '#'
                            s.send(cheating_data1)

                            cheating_data2 = "advance"
                            for x in range(BUFFER_SIZE - len("advance")):
                                cheating_data2 += '#'
                            s.send(cheating_data2)

                            cheating_data3 = "take food"
                            for x in range(BUFFER_SIZE - len("take food")):
                                cheating_data3 += '#'
                            s.send(cheating_data3)

                            cheating_data4 = "take linemate"
                            for x in range(BUFFER_SIZE - len("take linemate")):
                                cheating_data4 += '#'
                            s.send(cheating_data4)

                            cheating_data5 = "take deraumere"
                            for x in range(BUFFER_SIZE - len("take deraumere")):
                                cheating_data5 += '#'
                            s.send(cheating_data5)

                            cheating_data6 = "take sibur"
                            for x in range(BUFFER_SIZE - len("take sibur")):
                                cheating_data6 += '#'
                            s.send(cheating_data6)

                            data = "take mendiane"
                            for x in range(BUFFER_SIZE - len("take mendiane")):
                                data += '#'
                            # cheating_data7 = "take mendiane"
                            # for x in range(BUFFER_SIZE - len("take mendiane")):
                            #     cheating_data7 += '#'
                            #     s.send(cheating_data7)
                            #
                            # cheating_data8 = "take phiras"
                            # for x in range(BUFFER_SIZE - len("take phiras")):
                            #     cheating_data8 += '#'
                            # s.send(cheating_data8)
                            #
                            # data = "take thystame"
                            # for x in range(BUFFER_SIZE - len("take thystame")):
                            #     data += '#'
                        ###########################################################
                        elif event.key == pygame.K_d:
                            data = "incantation"
                            for x in range(BUFFER_SIZE - len("incantation")):
                                data += '#'
                        elif event.key == pygame.K_f:
                            data = "fork"
                            for x in range(BUFFER_SIZE - len("fork")):
                                data += '#'
                        elif event.key == pygame.K_SPACE:
                            data = "connect_nbr"
                            for x in range(BUFFER_SIZE - len("connect_nbr")):
                                data += '#'
                        # elif event.key == pygame.K_KP_ENTER:
                        #     data = "connect_nbr"
                        #     for x in range(BUFFER_SIZE - len("connect_nbr")):
                        #         data += '#'

                        else:
                            data = "broadcast hi"
                            for x in range(BUFFER_SIZE - len("broadcast hi")):
                                data += '#'
                        s.send(data)
        else:
            for event in pygame.event.get():
                keys = pygame.key.get_pressed()
                if event.type == pygame.QUIT:
                    # OVER = True
                    sys.exit(0)
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_KP_ENTER or event.key == pygame.K_RETURN:
                        start += 1
                        connect_game_server()
                        picture = pygame.transform.scale(pygame.image.load('./play.png'), (300, 200))
                        screen.blit(picture, (100, 50))  # 100, 50 -> is the starty point (x, y) from the top left (0, 0)
                    elif event.key == pygame.K_ESCAPE:
                        sys.exit(0)
        pygame.display.update()


        # except KeyboardInterrupt:
        #     print 'Interrupted.'
        #     s.close()
        #     break
main()
