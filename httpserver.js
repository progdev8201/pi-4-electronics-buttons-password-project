const express = require("express");
const amqp = require("amqplib");
const { response } = require("express");

const connection = amqp.connect("amqp://localhost");
const channelPromise = connection.then(conn => conn.createChannel());
const app = express();
const port = 3000;

let reqId = 0;
let responses = {};

app.use(express.urlencoded({ extended: true }));


app.post("/setPassword/:password", (req, resp) => {
    const requestKey = "" + reqId++;
    const action = "setPassword";
    const password = req.params.password;

    responses[requestKey] = resp;

    const response = { requestKey, action, password };

    channel.sendToQueue("get", Buffer.from(JSON.stringify(response)));
});

app.get("/getPassword", (req, resp) => {
    const requestKey = "" + reqId++;
    const action = "getPassword";

    responses[requestKey] = resp;

    console.log(requestKey);

    const response = { requestKey, action };

    channel.sendToQueue("get", Buffer.from(JSON.stringify(response)));
});

app.get("/getPasswordFoundCount", (req, resp) => {
    const requestKey = "" + reqId++;
    const action = "getPasswordFoundCount";

    responses[requestKey] = resp;

    const response = { requestKey, action };

    channel.sendToQueue("get", Buffer.from(JSON.stringify(response)));
});

app.get("/getPasswordFailedCount", (req, resp) => {
    const requestKey = "" + reqId++;
    const action = "getPasswordFailedCount";

    responses[requestKey] = resp;

    const response = { requestKey, action };

    channel.sendToQueue("get", Buffer.from(JSON.stringify(response)));
});

app.get("/getAllStats", (req, resp) => {
    const requestKey = "" + reqId++;
    const action = "getAllStats";

    responses[requestKey] = resp;

    const response = { requestKey, action };

    channel.sendToQueue("get", Buffer.from(JSON.stringify(response)));
});

app.post("/resetAllStats", (req, resp) => {
    const requestKey = "" + reqId++;
    const action = "resetAllStats";

    responses[requestKey] = resp;

    const response = { requestKey, action };

    channel.sendToQueue("get", Buffer.from(JSON.stringify(response)));
});


channelPromise.then(inChannel => {
    channel = inChannel;

    channel.assertQueue("get", { durable: false });
    channel.assertQueue("response", { durable: false });
});

channelPromise.then(() => {
    // vient setter un consumer
    channel.consume("response", (msg) => {
        const content = msg.content.toString();
        const contentObj = JSON.parse(content);

        channel.ack(msg);

        // find request then answer

        if (contentObj.status === "bad") {
            const message = { id: 'failure', errorMessage: contentObj.errorMsg };

            responses[contentObj.requestKey].status(404).send(message);
        } else {
            const message = { id: 'success', data: contentObj.data };

            responses[contentObj.requestKey].status(200).send(message);
        }

        delete responses[contentObj.requestKey];
    });
})

app.listen(port, () => {
    console.log('le serveur est parti et est pret a recevoir des requetes');
});