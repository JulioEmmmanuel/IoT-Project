const axios = require('axios');


let controller = {
    postMovimiento: (snapshot) => {
        const data = snapshot.val();
        
        if(data == 1){

            let datos = {
            lectura: data,
            idSensor: 1,
            }

            axios.post("http://localhost:8080/registro", datos);
        }
    },

    postTouch: (snapshot) => {
        const data = snapshot.val();
        
        if(data == 1){

            let datos = {
                lectura: data,
                idSensor: 2,
            }

            axios.post("http://localhost:8080/registro", datos);
        }
    },

    postKeypad: (snapshot) => {
        const data = snapshot.val();

        if(data != ""){
            
            let datos = {
                lectura: data,
                idSensor: 3,
            }

            axios.post("http://localhost:8080/registro", datos);
        }

    },

    postMovPhoto: (snapshot) => {
        const data = snapshot.val();

        if(data == 1){
        
            let datos = {
                capturaTimbre: 0
            }

            axios.post("http://localhost:8080/foto/mov", datos);
        }

    },

    postFacePhoto: (snapshot) => {
        const data = snapshot.val();

        if(data){

            console.log("url: " + data)

            let datos = {
                capturaTimbre: 1, 
                url: data        
            }

            axios.post("http://localhost:8080/foto/face", datos);
        }

    },    


}

module.exports = controller;