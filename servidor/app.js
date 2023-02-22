const express = require("express");
const bodyParser = require("body-parser");

const app = express();
const routes = require("./routes/api");

app.use(bodyParser.urlencoded({
    extended: false
}))

app.use(bodyParser.json({
    parameterLimit: 100000,
    limit: "50mb",
    extended: false
}))

app.use('', routes);


module.exports = app;