#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include <vector>

#include <signal.h>
#include <stdint.h>
#include <mosquitto.h>

#define mqtt_host "broker.hivemq.com"
#define mqtt_port 1883
#define client_id "telegram_bot"
#define input_topic "/robot_mai/order_manager/ch2"
#define output_topic "/robot_mai/order_manager/ch1"

#include <tgbot/tgbot.h>

using namespace std;
using namespace TgBot;

enum EStatus {MOVE_FORWARD, MOVE_BACK, TURN_RIGHT, TURN_LEFT};
bool flag_read = false;
int64_t currentChatId;
struct mosquitto* mosq;

string token = "YOUR TOKEN";

Bot bot(token);

void createOneColumnKeyboard(const vector<string>& buttonStrings, ReplyKeyboardMarkup::Ptr& kb)
{
	for (size_t i = 0; i < buttonStrings.size(); ++i) {
		vector<KeyboardButton::Ptr> row;
		KeyboardButton::Ptr button(new KeyboardButton);
		button->text = buttonStrings[i];
		row.push_back(button);
		kb->keyboard.push_back(row);
	}
}

void createKeyboard(const vector<vector<string>>& buttonLayout, ReplyKeyboardMarkup::Ptr& kb)
{
	for (size_t i = 0; i < buttonLayout.size(); ++i) {
		vector<KeyboardButton::Ptr> row;
		for (size_t j = 0; j < buttonLayout[i].size(); ++j) {
			KeyboardButton::Ptr button(new KeyboardButton);
			button->text = buttonLayout[i][j];
			row.push_back(button);
		}
		kb->keyboard.push_back(row);
	}
}

void anyMessageReply(Message::Ptr message) {
	if (StringTools::startsWith(message->text, "/start")) {
		return;
	}
	if (message->text == "Вперёд") {
		char data[] = "{ \"cmd\":\"forward\", \"val\": 0.1, \"spd\": 0.5}";
		mosquitto_publish(mosq, NULL, output_topic, strlen(data), data, 0, 0);
		bot.getApi().sendMessage(message->chat->id, "ok", false, 0);
		return;
	}
	if (message->text == "Назад") {
		char data[] = "{ \"cmd\":\"back\", \"val\": 0.1, \"spd\": 0.5}";
		mosquitto_publish(mosq, NULL, output_topic, strlen(data), data, 0, 0);
		bot.getApi().sendMessage(message->chat->id, "ok", false, 0);
		return;
	}
	if (message->text == "Вправо") {
		char data[] = "{ \"cmd\":\"right\", \"val\": 0.1, \"spd\": 0.5}";
		mosquitto_publish(mosq, NULL, output_topic, strlen(data), data, 0, 0);
		bot.getApi().sendMessage(message->chat->id, "ok", false, 0);
		return;
	}
	if (message->text == "Влево") {
		char data[] = "{ \"cmd\":\"left\", \"val\": 0.1, \"spd\": 0.5}";
		mosquitto_publish(mosq, NULL, output_topic, strlen(data), data, 0, 0);
		bot.getApi().sendMessage(message->chat->id, "ok", false, 0);
		return;
	}

	bot.getApi().sendMessage(message->chat->id, "Такой команды не существует");
}

int main() {
	setlocale(LC_ALL, "Russian");

	mosquitto_lib_init();
	mosq = mosquitto_new(client_id, true, 0);
	if (mosq) {
		mosquitto_connect_async(mosq, mqtt_host, mqtt_port, 60);
		mosquitto_loop_start(mosq);
		mosquitto_subscribe(mosq, NULL, input_topic, 0);
	}
	

	ReplyKeyboardMarkup::Ptr keyboardOneCol(new ReplyKeyboardMarkup);
	createOneColumnKeyboard({ "Хочу напиток", "На базу" }, keyboardOneCol);

	ReplyKeyboardMarkup::Ptr keyboardWithLayout(new ReplyKeyboardMarkup);
	createKeyboard({
	  {"Вперёд", "Назад"},
	  {"Влево", "Вправо"},
		{"Налить напиток"},
		}, keyboardWithLayout);

	bot.getEvents().onCommand("automatic", [&bot, &keyboardOneCol](Message::Ptr message) {
		bot.getApi().sendMessage(message->chat->id, "Включён режим автоматической поездки", false, 0, keyboardOneCol);
		});
	bot.getEvents().onCommand("manual", [&bot, &keyboardWithLayout](Message::Ptr message) {
		bot.getApi().sendMessage(message->chat->id, "Включён режим ручного управления", false, 0, keyboardWithLayout);
		});
	bot.getEvents().onAnyMessage([&bot](Message::Ptr message) {
		printf("User wrote %s\n", message->text.c_str());
	if (StringTools::startsWith(message->text, "/automatic") || StringTools::startsWith(message->text, "/manual")) {
		return;
	}
	bot.getApi().sendMessage(message->chat->id, ": " + message->text);
		});

	bot.getEvents().onAnyMessage(anyMessageReply);

	vector<BotCommand::Ptr> commands;
	BotCommand::Ptr cmdArray(new BotCommand);
	cmdArray->command = "manual";
	cmdArray->description = "Включить ручное управление";

	commands.push_back(cmdArray);

	cmdArray = BotCommand::Ptr(new BotCommand);
	cmdArray->command = "automatic";
	cmdArray->description = "Включить автоматический режим";
	commands.push_back(cmdArray);

	bot.getApi().setMyCommands(commands);

	vector<BotCommand::Ptr> vectCmd;
	vectCmd = bot.getApi().getMyCommands();

	signal(SIGINT, [](int s) {
		printf("SIGINT got\n");
	exit(0);
		});

	try {
		printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
		bot.getApi().deleteWebhook();

		TgLongPoll longPoll(bot);
		while (true) {
			printf("Long poll started\n");
			longPoll.start();
		}
	}
	catch (exception& e) {
		printf("error: %s\n", e.what());
	}

	return 0;
}

