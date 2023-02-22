let con = require("../model/timbre_inteligente.js")

let controller = {
    crear_registro: (req, res)=>{
        
        let reg = req.body

        let values = [[reg.date, reg.lectura, reg.idSensor]];

        con.query("INSERT INTO registrosensor(tiempoRegistro, lectura, idSensor) VALUES ?", [values], (err, result) => {
            if(err) console.log(err)
            return res.status(201).json({status: 201})
        });
    },

    crear_registro_foto_mov: (req, res)=>{
        
        let reg = req.body

        let values = [[reg.capturaTimbre, reg.date]];

        con.query("INSERT INTO registrosfotos(capturaDeTimbre, tiempoCaptura) VALUES ?", [values], (err, result) => {
            if(err) console.log(err)
            return res.status(201).json({status: 201})
        });
    },

    crear_registro_foto_face: (req, res)=>{
        
        let reg = req.body

        if(reg.user_id == -1){
            let values = [[reg.capturaTimbre, reg.date]];

            con.query("INSERT INTO registrosfotos(capturaDeTimbre, tiempoCaptura) VALUES ?", [values], (err, result) => {
                if(err) console.log(err)
                return res.status(201).json({status: 201})
            });
        } else {
            let values = [[reg.capturaTimbre, reg.date, reg.user_id]];

            con.query("INSERT INTO registrosfotos(capturaDeTimbre, tiempoCaptura, idUsuario) VALUES ?", [values], (err, result) => {
                if(err) console.log(err)
                return res.status(201).json({status: 201})
            });
        }

    }


}

module.exports = controller;