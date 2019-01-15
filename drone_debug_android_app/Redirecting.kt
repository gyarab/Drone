package com.example.minion.dronebtcontroller

private var communicationData:StartCommunication? = null
fun setCommunicationData(comData:StartCommunication){
    communicationData = comData
}
fun getCommunicationData():StartCommunication?{
    return communicationData
}