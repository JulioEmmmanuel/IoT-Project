const {
  SinricPro, startSinricPro, raiseEvent, eventNames,
} = require('sinricpro');

const database = require("../model/firebase_realtime_db")

const APPKEY = 'f42b81f8-f8d9-4086-ac22-f512c3617e78';
const APPSECRET = '698ede6d-a2d2-4a6a-ab7b-3801ef35e755-9f322dca-06c2-492d-bff7-48c910ae8798';
const motor = '6364690b333d12dd2ae52602';
const deviceIds = [motor];

function setPowerState(deviceid, data) {
  const switchRef = database.ref("alexa/switch")
    if(data == "On"){
        switchRef.set(1)
    } else {
        switchRef.set(0)
    }
  return true;
}

const sinricpro = new SinricPro(APPKEY, deviceIds, APPSECRET, true);
const callbacks = { setPowerState };

startSinricPro(sinricpro, callbacks);

module.exports = sinricpro;