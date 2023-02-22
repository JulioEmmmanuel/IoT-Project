const admin = require("firebase-admin");
const serviceAccount = require("../serviceAccountKey.json");
const {JWT} = require('google-auth-library');


let FirebaseController = require("../controllers/firebase_vals");

function getAccessToken() {
  return new Promise(function(resolve, reject) {
    const jwtClient = new JWT(
      serviceAccount.client_email,
      null,
      serviceAccount.private_key,
      ['https://www.googleapis.com/auth/cloud-platform'],
      null
    );
    jwtClient.authorize(function(err, tokens) {
      if (err) {
        reject(err);
        return;
      }
      resolve(tokens.access_token);
    });
  });
}

const firebaseConfig = {
    credential: admin.credential.cert(serviceAccount),
    databaseURL: "https://timbre-inteligente-b20fd-default-rtdb.firebaseio.com/",
  };

// Initialize Firebase
admin.initializeApp(firebaseConfig);

// Initialize Realtime Database and get a reference to the service
let database = admin.database();

let access_token = getAccessToken();

//Guardar datos del sensor de movimiento
const movRef = database.ref('sensores/movimiento');
movRef.on('value', (snapshot) => {
  FirebaseController.postMovimiento(snapshot);
}, (errorObject) => {
  console.log("The read failed: " + errorObject.name);
});

//Guardar datos del touchpad
const touchRef = database.ref('sensores/tactil');
touchRef.on('value', (snapshot) => {
  FirebaseController.postTouch(snapshot);
}, (errorObject) => {
  console.log("The read failed: " + errorObject.name);
});

//Guardar datos del keypad
const keypadRef = database.ref('sensores/passAttempt');
keypadRef.on('value', (snapshot) => {
  FirebaseController.postKeypad(snapshot);
}, (errorObject) => {
  console.log("The read failed: " + errorObject.name);
});

//Guardar registros fotos por movimiento
const fotoMovRef = database.ref('fotos/movimiento');
fotoMovRef.on('value', (snapshot) => {
  FirebaseController.postMovPhoto(snapshot);
}, (errorObject) => {
  console.log("The read failed: " + errorObject.name);
});

//Guardar registros fotos rostros
const fotoRostroRef = database.ref('fotos/url');
fotoRostroRef.on('value', (snapshot) => {
  FirebaseController.postFacePhoto(snapshot);
}, (errorObject) => {
  console.log("The read failed: " + errorObject.name);
});



module.exports = database;