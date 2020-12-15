#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <nlohmann/json.hpp>
#include "tp2/Program.hpp"
#include <wiringPi.h>

void sendJsonToResponseQueue(nlohmann::json jsonResponse, AmqpClient::Channel::ptr_t channel)
{
    AmqpClient::BasicMessage::ptr_t msg = AmqpClient::BasicMessage::Create(jsonResponse.dump(8));

    channel->BasicPublish("", "response", msg);
}

void handleAction(Program &program, nlohmann::json jsonRequest, AmqpClient::Channel::ptr_t channel)
{
    std::string action = jsonRequest["action"];
    nlohmann::json jsonResponse;

    jsonResponse["requestKey"] = jsonRequest["requestKey"];

    if (action == "setPassword")
    {
        std::string password = jsonRequest["password"];

        if (program.isPasswordValid(password))
        {
            program.setPassword(password);
            jsonResponse["status"] = "ok";
            sendJsonToResponseQueue(jsonResponse, channel);
        }
        else
        {
            jsonResponse["status"] = "bad";
            jsonResponse["errorMsg"] = "Password is not valid!";
            sendJsonToResponseQueue(jsonResponse, channel);
        }
    }
    else if (action == "getPassword")
    {
        jsonResponse["status"] = "ok";
        jsonResponse["data"] = program.password();
        sendJsonToResponseQueue(jsonResponse, channel);
    }
    else if (action == "getPasswordFoundCount")
    {
        jsonResponse["status"] = "ok";
        jsonResponse["data"] = program.passwordFoundCount();
        sendJsonToResponseQueue(jsonResponse, channel);
    }
    else if (action == "getPasswordFailedCount")
    {
        jsonResponse["status"] = "ok";
        jsonResponse["data"] = program.passwordFailedCount();
        sendJsonToResponseQueue(jsonResponse, channel);
    }
    else if (action == "getAllStats")
    {
        jsonResponse["status"] = "ok";
        jsonResponse["data"] = {program.passwordFoundCount(), program.passwordFailedCount()};
        sendJsonToResponseQueue(jsonResponse, channel);
    }
    else if (action == "resetAllStats")
    {
        jsonResponse["status"] = "ok";
        program.resetStats();
        sendJsonToResponseQueue(jsonResponse, channel);
    }
    else
    {
        jsonResponse["status"] = "bad";
        jsonResponse["errorMsg"] = "This action does not exist!";
        sendJsonToResponseQueue(jsonResponse, channel);
    }
}

void receive(Program &program)
{
    //ouvir une connection
    AmqpClient::Channel::OpenOpts opts = AmqpClient::Channel::OpenOpts::FromUri("amqp://localhost");
    AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Open(opts);

    //creer le channel get set
    channel->DeclareQueue("get", false, false, false, false);
    channel->DeclareQueue("reponse", false, false, false, false);

    //ajouter les channels
    std::string tagGet = channel->BasicConsume("get");

    while (1)
    {
        {
            AmqpClient::Envelope::ptr_t envelope;
            bool msgRecu = channel->BasicConsumeMessage(tagGet, envelope, 0);

            if (msgRecu)
            {
                nlohmann::json jsonRequest = nlohmann::json::parse(envelope->Message()->Body());

                handleAction(program, jsonRequest, channel);
            }
        }

        //Sleep pour economiser le cpu un peu
        std::this_thread::sleep_for(std::chrono::milliseconds{5});
    }
}

int main()
{
    wiringPiSetup();

    std::string password = "AAA";

    Program program{password};

    std::thread thr{receive, std::ref(program)};

    program.run();

    thr.join();

    return 0;
}