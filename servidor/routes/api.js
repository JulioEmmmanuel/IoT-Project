const express = require("express");
const api = express.Router();
const {body} = require("express-validator");
const middleware = require("../middlewares/processing")

let SensoresController = require("../controllers/sensores");
api.post("/registro", middleware.generar_fecha, SensoresController.crear_registro);
api.post("/foto/mov", middleware.generar_fecha, SensoresController.crear_registro_foto_mov);
api.post("/foto/face", middleware.generar_fecha, middleware.reconocer_rostro, middleware.marcar_reconocido, middleware.enviar_notification, SensoresController.crear_registro_foto_face);

module.exports = api;