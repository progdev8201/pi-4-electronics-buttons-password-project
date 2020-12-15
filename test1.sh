curl -X POST 'http://localhost:3000/setPassword/BBA' && echo;
curl -X POST 'http://localhost:3000/setPassword/BBsdfA' && echo;
curl -i http://localhost:3000/getPassword && echo;
curl -i http://localhost:3000/getPasswordFoundCount && echo;
curl -i http://localhost:3000/getPasswordFailedCount && echo;
curl -i http://localhost:3000/getAllStats && echo;
curl -X POST 'http://localhost:3000/resetAllStats' && echo;
curl -i http://localhost:3000/getAllStats && echo;