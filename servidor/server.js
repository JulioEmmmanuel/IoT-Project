const app = require("./app");
const con = require("./model/timbre_inteligente");
const firedb = require("./model/firebase_realtime_db");
const sinric = require("./sinric _pro/initializer")


port = process.env.PORT || 8080;


con.connect(function(err){
    if(err) throw err;
    console.log("Conexión a la base de datos establecida con éxito");

    let server = app.listen(port, ()=>{
        console.log(`Servidor corriendo correctamente en la url http://localhost: ${port}`);
    })
        
})


