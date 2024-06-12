const express = require('express')
var cors = require('cors');
const app = express()
app.use(cors());
const port = 3001

const merchant_model = require('./merchant_model')

app.use(express.json())


app.get('/', (req, res) => {
  merchant_model.getMerchants()
  .then(response => {
    res.status(200).send(response);
  })
  .catch(error => {
    res.status(500).send(error);
  })
})

app.post('/compare_radius', (req, res) => {
  merchant_model.radius_filter(req.body)
  .then(response => {
    res.send(response);
  })
  .catch(error => {
    res.send(error);
  })
})

app.post('/uav_values', (req, res) => {
  merchant_model.uav_values(req.body)
  .then(response => {
    console.log(response);
    res.send(response);
  })
  .catch(error => {
    res.send(error);
  })
})

app.post('/merchants', (req, res) => {
  merchant_model.createMerchant(req.body)
  .then(response => {
    res.status(200).send(response);
  })
  .catch(error => {
    res.status(500).send(error);
  })
})

app.delete('/merchants/:id', (req, res) => {
  merchant_model.deleteMerchant(req.params.id)
  .then(response => {
    res.status(200).send(response);
  })
  .catch(error => {
    res.status(500).send(error);
  })
})
app.listen(port, () => {
  console.log(`App running on port ${port}.`)
})