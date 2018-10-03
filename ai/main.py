import sys
import socket
import select

BUF_SIZE = 8192
TAKE_FOOD_CYCLE = 1200
TAKE_RESOURCE_CYCLE = 1200
GO_THROUGH_MAP = 1
S = []

def msg_padding (str):

    for x in range(BUF_SIZE - len(str)):
        str += '#'
    return str

def init():

    team_name = raw_input("Enter the team name you want to join...\n")
    while True:
        if ' ' not in team_name and team_name != '':
            break
        team_name = raw_input("Enter a valid team name...\n")
    host_ip = raw_input("Enter the host ip, if empty, 127.0.0.1 will be used...\n")
    while True:
        if host_ip == '':
            host_ip = '127.0.0.1'
        if socket.inet_aton(host_ip) != 0:
            break
        host_ip = raw_input("Enter a valid host ip...\n")
    return team_name, host_ip

def connect(team_name, host_ip):

    port = 4242
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    S.append(s)
    try:
        s.connect((host_ip, port))
    except Exception as e:
        print("Connection Error: Connection refused")
        sys.exit(1)
    team_name = msg_padding(team_name)
    s.send(team_name)
    data = s.recv(BUF_SIZE)
    data += s.recv(BUF_SIZE)
    data += s.recv(BUF_SIZE)
    if "FULL" in data or "NOT FOUND" in data:
        print(data.replace("#", ""))
        sys.exit(1)
    data = data.replace("#", "")
    data = data.split("\n")[1]
    data = data.split(" ")
    limit = max(data)
    data = msg_padding("take food")
    s.send(data)
    return limit

def get_cmd(state):

    if state == 1:
        return msg_padding("take food")
    elif state == 2:
        return msg_padding("take linemate")
    elif state == 3:
        return msg_padding("take deraumere")
    elif state == 4:
        return msg_padding("take sibur")
    elif state == 5:
        return msg_padding("take mendiane")
    elif state == 6:
        return msg_padding("take phiras")
    elif state == 7:
        return msg_padding("take thystame")
    elif state == 8:
        return msg_padding("see")
    elif state == 9:
        return msg_padding("incantation")
    elif state == 10:
        return msg_padding("fork")
    elif state == 11:
        return msg_padding("kick")
    elif state == 12:
        return msg_padding("inventory")
    elif state == 13:
        return msg_padding("advance")
    elif state == 14:
        return msg_padding("left")
    elif state == 15:
        return msg_padding("right")

def main():
    team_name, host_ip = init()
    limit = int(connect(team_name, host_ip))
    count = 0
    state = 0
    win_c = 0
    for x in range(5):
        connect(team_name, host_ip)
    while True:
        readable, writable, exceptional = select.select(S, S, [], 0.0)
        for i in readable:
            data = ""
            stop = False
            while not stop:
                data += i.recv(BUF_SIZE)
                if not data:
                    sys.exit(1)
                data = data.replace("#", "")
                stop = "KO" in data or "OK" in data or "elevation" in data or "level" or "WON" in data
                if "WON" in data:
                    win_c += 1
                    if win_c > 5:
                        sys.exit(1)
            print (data)
            if count < TAKE_FOOD_CYCLE:
                if "TAKE OK" in data or "ADVANCE OK" in data:
                    if "TAKE OK" in data:
                        count += 1
                    data = get_cmd(1)
                else:
                    data = get_cmd(13)
            elif count < TAKE_FOOD_CYCLE + TAKE_RESOURCE_CYCLE:
                if "TAKE OK" in data or "ADVANCE OK" in data:
                    count += 1
                    data = get_cmd(count % 7 + 1)
                else:
                    data = get_cmd(13)
            elif count < TAKE_FOOD_CYCLE + TAKE_RESOURCE_CYCLE + (limit*2+4)*limit*GO_THROUGH_MAP and ("KO" in data or "OK" in data or "moving" in data or "received" in data):
                if i == S[0]:
                    if state % (limit * 2 + 4) == limit * 2:
                        data = get_cmd(15)  # right
                    elif state % (limit * 2 + 4) == limit * 2 + 1:
                        data = get_cmd(11)  # kick
                    elif state % (limit * 2 + 4) == limit * 2 + 2:
                        data = get_cmd(13)  # advance
                    elif state % (limit * 2 + 4) == limit * 2 + 3:
                        data = get_cmd(14)  # left
                    elif state % 2 == 0:
                        data = get_cmd(11)  # kick
                    else:
                        data = get_cmd(13)  # advance
                    state += 1
                    count += 1
                else:
                    data = get_cmd(1)
            elif count < TAKE_FOOD_CYCLE + TAKE_RESOURCE_CYCLE + (limit*2+4)*limit*GO_THROUGH_MAP + 10:
                if i == S[0]:
                    if "KO" in data or "OK" in data or "level" in data:
                        data = get_cmd(9)
                    else:
                        data = ""
                else:
                    data = ""
            else:
                data = ""
            if data is not "":
                print("to sent:" + data.replace("#", "") + " count = " + str(count))
                i.send(data)

if __name__ == '__main__':
    main()
