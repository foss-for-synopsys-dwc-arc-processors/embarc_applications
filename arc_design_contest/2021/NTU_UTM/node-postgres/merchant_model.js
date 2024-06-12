const Pool = require('pg').Pool
const pool = new Pool({
  user: 'postgres',
  host: '140.112.94.38',
  database: 'GIS',
  password: '19980107',
  port: 5432,
});

const getMerchants = () => {
    return new Promise(function(resolve, reject) {
      pool.query('SELECT * FROM public.gis', (error, results) => {
        if (error) {
          reject(error)
        }
        resolve(results.rows);
      })
    }) 
  }

const radius_filter = (body) =>{
  return new Promise(function(resolve, reject){
    const coor = body.coordinates;
    const radius = body.radius;
    
    pool.query('select p_pm25, ST_AsGeoJSON(geom) as point from public.gis2 \
                 cross join (select ST_MakePoint($1, $2)::geography as ref_geom) as r \
                 where ST_DWithin(geom, ref_geom, $3)', [coor[0], coor[1], radius],(error, results) => {
                   if (error) {
                     reject(error)
                   }
                   resolve(results.rows);
                 })
  })
}

const uav_values = (body) =>{
  return new Promise(function(resolve, reject){
    const coor = body.coordinates;
    const radius = body.radius;
    
    pool.query('select p_pm25, height, ST_AsGeoJSON(geom) as point from public.gis \
                 cross join (select ST_MakePoint($1, $2)::geography as ref_geom) as r \
                 where ST_DWithin(geom, ref_geom, $3)', [coor[0], coor[1], radius],(error, results) => {
                   if (error) {
                     reject(error)
                   }
                   resolve(results.rows);
                 })
  })
}

const createMerchant = (body) => {
    return new Promise(function(resolve, reject) {
      const { name, email } = body
      pool.query('INSERT INTO lamp_da (pm25) VALUES ($1) RETURNING *', [pm25], (error, results) => {
        if (error) {
          reject(error)
        }
        resolve(`A new merchant has been added added: ${results.rows[0]}`)
      })
    })
  }

  const deleteMerchant = () => {
    return new Promise(function(resolve, reject) {
      const id = parseInt(request.params.id)
      pool.query('DELETE FROM lamp_da WHERE id = $1', [id], (error, results) => {
        if (error) {
          reject(error)
        }
        resolve('lamp_da deleted with ID: ${id}')
      })
    })
  }
  
  module.exports = {
    getMerchants,
    radius_filter,
    uav_values,
    createMerchant,
    deleteMerchant,
  }