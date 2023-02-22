const {spawn} = require("child_process")
const database = require("../model/firebase_realtime_db")
const { getMessaging } = require("firebase-admin/messaging");


const middlewares = {

    generar_fecha: (req, res, next)=>{
            
        let ts = Date.now();
        let date_ob = new Date(ts);
        let day = date_ob.getDate();
        let month = date_ob.getMonth() + 1;
        let year = date_ob.getFullYear();
        let hours = date_ob.getHours();
        let minutes = date_ob.getMinutes();
        let seconds = date_ob.getSeconds();

        let date_string = year + "-" + month + "-" + day + " " + hours + ":" + minutes + ":" + seconds;
        req.body.date = date_string;
        next();   
    },

    reconocer_rostro: (req, res, next) => {

        let dataToSend;

        //crear el proceso hijo para reconocimiento facial
        console.log(req.body.url)

        const python = spawn("python3", ["C:/Users/USUARIO DELL/Documents/IoT/servidor/reconocimiento facial/rec_one.py", req.body.url]);

        //recolectamos la respuesta
        python.stdout.on('data', function(data){
            dataToSend = data.toString();
        });

        python.stderr.on('data', data => {
            console.error(`sterr: ${data}`);
            next();

        });

        // En caso de que se cierre nos encargamos de que el stream del proceso hijo sea cerrado

        python.on('exit', (code) => {
            console.log(`child process exited with code ${code}, ${dataToSend}`);
            if(dataToSend){
                req.body.user_id = dataToSend.trim();
            } else {
                req.body.user_id = -1;
            }
            next();
        })

    },

    marcar_reconocido: (req, res, next) => {
        const recRef = database.ref("fotos/reconocido")
        if(req.body.user_id != -1){
            recRef.set(1)
        } else {
            recRef.set(0)
        }
        next();
    },

    enviar_notification: (req, res, next) => {

        const message = {
            notification: {
                title: 'Alerta',
                body: 'Hay alguien frente a tu puerta'
            },
            android: {
                notification: {
                icon: 'eye',
                color: '#3F51B5'
                }
            },
            token: "fNjSepWSSqCR2CmisedwXn:APA91bFIKbI4Lvp9jWXkAfyxsM0dHL3GWNcXxQgRCMI9Ct-fQ6o3tCxM8QIiwLUEJRVKVN8492_C0FcInUr9xiFLTc0goKo3fHPjIW075vM2J4WvUfajIkErjQo0Foe9pRcexKBl9Bel"
        };

        getMessaging().send(message)
        .then((response) => {
            // Response is a message ID string.
            console.log('Successfully sent message:', response);
        })
        .catch((error) => {
            console.log('Error sending message:', error);
        });

        next();

    }

}

module.exports = middlewares;