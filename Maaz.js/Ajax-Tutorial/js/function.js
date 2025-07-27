function SendXML(method, href, type, param = "") {
    this.xhr = new XMLHttpRequest();
    this.xhr.open(method, href, true);

    let params = (method == "POST" ? param : "");
    console.log("Parameters :", params);

    this.xhr.getResponseHeader("Content-Type", type);
    this.xhr.send(params);

    return this;
}