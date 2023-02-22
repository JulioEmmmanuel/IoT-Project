const mysql = require("mysql");

var con = mysql.createConnection({
    host: "localhost",
    port: 3306,
    user: "root",
    database: "timbre_inteligente"
})

module.exports = con;