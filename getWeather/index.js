const http = require("http");
const htmlparser2 = require("htmlparser2");

setInterval(function () {
  const optionsWeather = {
    hostname: "weather",
    port: 80,
    path: "/livedata.htm",
    method: "GET",
  };

  httpGet(optionsWeather, function (response) {
    const optionsCloudPush = {
      hostname: "www.kai-merklein.de",
      port: 80,
      path: "/avrtl/updweather.php?station=1" + parse(response),
      method: "GET",
    };
    httpGet(optionsCloudPush, function (response) {
      console.log(response);
    });
  });
}, 60000);

function parse(stp) {
  var outURLParam = "";
  const parser = new htmlparser2.Parser(
    {
      onopentag(name, attribs) {
        if (name === "input") {
          //console.log(`${name}, ${attribs.name}: ${attribs.value}`);
          if (attribs.name == "inTemp")
            outURLParam = outURLParam + "&it=" + attribs.value;
          if (attribs.name == "outTemp")
            outURLParam = outURLParam + "&ot=" + attribs.value;
          if (attribs.name == "inHumi")
            outURLParam = outURLParam + "&ih=" + attribs.value;
          if (attribs.name == "outHumi")
            outURLParam = outURLParam + "&oh=" + attribs.value;
          if (attribs.name == "windir")
            outURLParam = outURLParam + "&wd=" + attribs.value;
          if (attribs.name == "avgwind")
            outURLParam = outURLParam + "&wf=" + attribs.value;
          if (attribs.name == "gustspeed")
            outURLParam = outURLParam + "&gs=" + attribs.value;
          if (attribs.name == "solarrad")
            outURLParam = outURLParam + "&so=" + attribs.value;
          if (attribs.name == "rainofhourly")
            outURLParam = outURLParam + "&rh=" + attribs.value;
          if (attribs.name == "rainofdaily")
            outURLParam = outURLParam + "&rd=" + attribs.value;
          if (attribs.name == "RelPress")
            outURLParam = outURLParam + "&pr=" + attribs.value;
        }
      },
      ontext(text) {
        //console.log("-->", text);
      },
      onclosetag(tagname) {},
    },
    { decodeEntities: true }
  );
  parser.write(stp);
  parser.end();
  return outURLParam;
}

function httpGet(options, callback) {
  let response = "";

  const req = http.request(options, function (res) {
    //console.log("STATUS: " + res.statusCode);
    //console.log("HEADERS: " + JSON.stringify(res.headers));
    res.setEncoding("utf8");
    res.on("data", function (chunk) {
      response = response + chunk;
    });
    res.on("end", function () {
      callback(response);
    });
  });

  req.on("error", function (e) {
    console.log("problem with request: " + e.message);
  });

  req.end();
}
