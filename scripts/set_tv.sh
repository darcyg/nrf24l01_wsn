#!/bin/bash

val=$1
size=${#val}

host="192.168.0.53"
topic="home/tv"

case $val in
    start)
    mosquitto_pub -h $host -t $topic -m "s03F"
    ;;
    stop)
    mosquitto_pub -h $host -t $topic -m "s03D"
    ;;
    mute)
    mosquitto_pub -h $host -t $topic -m "s00D"
    ;;
    vp)
        for (( c=0; c<$2; c++ )); do
            mosquitto_pub -h $host -t $topic -m "s010"
            sleep 0.2
        done
    ;;
    vm)
        for (( c=0; c<$2; c++ )); do
            mosquitto_pub -h $host -t $topic -m "s011"
            sleep 0.2
        done
    ;;
    hdmi1)
    mosquitto_pub -h $host -t $topic -m "s0F9"
    ;;
    hdmi2)
    mosquitto_pub -h $host -t $topic -m "t0FB"
    ;;
    side)
    mosquitto_pub -h $host -t $topic -m "s0C6"
    ;;
    apple)
        case $2 in
            up)
            mosquitto_pub -h $host -t $topic -m "a77E1507C"
            ;;
            down)
            mosquitto_pub -h $host -t $topic -m "a77E1307C"
            ;;
            left)
            mosquitto_pub -h $host -t $topic -m "a77E1907C"
            ;;
            right)
            mosquitto_pub -h $host -t $topic -m "a77E1607C"
            ;;
            enter)
            mosquitto_pub -h $host -t $topic -m "a77E13A7C"
            sleep 0.3;
            mosquitto_pub -h $host -t $topic -m "a77E1A07C"
            ;;
            play)
            mosquitto_pub -h $host -t $topic -m "a77E1FA7C"
            sleep 0.3;
            mosquitto_pub -h $host -t $topic -m "a77E1A07C"
            ;;
            menu)
            mosquitto_pub -h $host -t $topic -m "a77E1C07C"
            ;;
            *)
            echo "error"
            ;;
    esac
    ;;
    canal)
        case $2 in
            up)
            mosquitto_pub -h $host -t $topic -m "c458022"
            ;;
            down)
            mosquitto_pub -h $host -t $topic -m "c458026"
            ;;
            left)
            mosquitto_pub -h $host -t $topic -m "c458024"
            ;;
            right)
            mosquitto_pub -h $host -t $topic -m "c458020"
            ;;
            enter)
            mosquitto_pub -h $host -t $topic -m "c45802a"
            ;;
            play)
            mosquitto_pub -h $host -t $topic -m "c458036"
            ;;
            pause)
            mosquitto_pub -h $host -t $topic -m "c45803A"
            ;;
            stop)
            mosquitto_pub -h $host -t $topic -m "c458038"
            ;;
            # mute)
            # mosquitto_pub -h $host -t "salon/ir" -m "c45800d"
            # ;;
            return)
            mosquitto_pub -h $host -t $topic -m "c45801c"
            ;;
            sortie)
            mosquitto_pub -h $host -t $topic -m "c45801b"
            ;;
            menu)
            mosquitto_pub -h $host -t $topic -m "c458019"
            ;;
            power)
            mosquitto_pub -h $host -t $topic -m "c45800c"
            ;;
            guide)
            mosquitto_pub -h $host -t $topic -m "c45801a"
            ;;
            vp)
                for (( c=0; c<$3; c++ )); do
                    mosquitto_pub -h $host -t $topic -m "c458012"
                    sleep 0.2
                done
            ;;
            vm)
                for (( c=0; c<$3; c++ )); do
                    mosquitto_pub -h $host -t $topic -m "c458011"
                    sleep 1
                done
            ;;
            *)
            echo "error"
            ;;
    esac
    ;;
    pp)
    mosquitto_pub -h $host -t $topic -m "c458014"
    ;;
    pm)
    mosquitto_pub -h $host -t $topic -m "c458013"
    ;;
    *)
    for (( c=0; c<$size; c++ )); do
        mosquitto_pub -h $host -t $topic -m "c45800${val:$c:1}"
        sleep 0.2
    done
    ;;
esac
