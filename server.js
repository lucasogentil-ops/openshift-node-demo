const express = require('express'); 
const app=express();

const PORT = process.env.PORT || 8080;

app.get('/', (req, res) => {
    res.status(200).json({
        message: "Welcome Tony and Kate to my first API deployed on OpenShift",
        developer: "Lucas",
        status: "success"
    });
});


app.get("/health", (req, res) => {
    res.status(200).json({
        status: "healthy",
        message: "Application is running just fine"
    });
});



app.listen(PORT, "0.0.0.0", ()=>{
    console.log(`Server is running on port: ${PORT}`);
});

