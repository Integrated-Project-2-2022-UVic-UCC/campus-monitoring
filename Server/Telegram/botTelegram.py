import time
import telebot
import mysql.connector
import json
import os
from telebot import types
from datetime import datetime


def getKeys():
    file = open(os.path.abspath("users.json"))
    dicConfig = json.load(file)
    file.close()
    return dicConfig


def saveKeys(c):
    with open(os.path.abspath("users.json"), 'w') as outfile:
        json.dump(chat_id, outfile)


chat_id = getKeys()

# https://api.telegram.org/bot867573955:AAEJUO1URD6ICiinQ-sr_kEPnmuJ2dCMgNs/getUpdates
token = '5243971101:AAGt-ISFTnX6lpEmZfVBba7sHZeWVWdD5qI'
try:
    tb = telebot.TeleBot(token)
    print('ok')
except:
    print('error iniciacio')

time.sleep(10)


def enviar_missatge(idtelebot, missatge):
    for i in chat_id:
        if str(idtelebot) == i:
            print(str(datetime.now()) + ' , ' + str(idtelebot))
            tb.send_message(str(idtelebot), text=str(missatge), parse_mode="Markdown")


@tb.message_handler(commands=['start'])
def Start(message):
    missatge = str(message.chat.first_name) + '\n'
    missatge += str(message.chat.id)
    try:
        tb.send_message('743717839', text=str(missatge), parse_mode="Markdown")
    except:
        print("error sent message Start")


@tb.message_handler(commands=['info'])
def info(message):
    missatge = "*INFORMACIÓ* \n"
    missatge += "/getUsers \n"
    missatge += "/updateUser \n"
    missatge += "/deleteUser \n"
    missatge += "/start \n"
    sendMessageAdmin(message.chat.id, missatge)


error_counter = 0
markup = types.ReplyKeyboardMarkup(row_width=1)
b1 = types.KeyboardButton('/AirQ')
b2 = types.KeyboardButton('/OTR')
b3 = types.KeyboardButton('/ArrencadorsAspiracio')
b4 = types.KeyboardButton('/Compressor')
b5 = types.KeyboardButton('/Biomassa')
b6 = types.KeyboardButton('/AlarmesBiomassa')
b7 = types.KeyboardButton('/')
b8 = types.KeyboardButton('/')
markup.add(b3, b2, b4, b1, b5, b6) #, b5, b6, b7, b8)


print('polling', datetime.now())
while(True):
    try:
        tb.polling(none_stop=True, interval=0, timeout=60)
    except KeyboardInterrupt:
        raise
    except:
        print('error')




