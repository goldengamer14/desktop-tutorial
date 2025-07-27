// API key for news.org
// 268386f0fbaa4711a216a05f86c686ce

// A function to GET the News using XMLHttpRequest
function getNewsApi() {
    // This is the url from where im gonna get my news from
    const url = "https://newsapi.org/v2/top-headlines?sources=bbc-news&" +
        "apiKey=268386f0fbaa4711a216a05f86c686ce";


    // I've made this function in another file function.js to ease the use of xhr
    SendXML("GET", url, "application/json", "").xhr.onload = function () {
        let news = JSON.parse(this.response);
        let articles = news.articles;

        new Promise((resolve, reject) => {
            let a = document.getElementsByClassName("newsHeadline");
            let nonClickable = function () {
                Array.from(a).forEach(function (e, i) {
                    e.onclick = function (f) {
                        f.preventDefault();
                        const mainNews = document.getElementsByClassName("mainNews");
                        mainNews[i].open == true ? mainNews[i].open = false : mainNews[i].open = true;
                    }
                });
            }
            resolve(nonClickable);
        }).then(fun => fun());

        for (let a of articles) {

            let myNews = `<h3>
            <details class="mainNews wid-fit-content">
    
                <summary class="cur-pointer">

                    <span style="color: aliceblue">Breaking News ${articles.indexOf(a) + 1}:</span>
                    <a href="" target="_self" class="newsHeadline">${a.title}</a>

                </summary>
                <span class="newsShow">
                    <span style="color: bisque;">Description: ${a.description}</span><br>
                    Content: ${a.content}.<br>
                    <a href="${a.url}" style="display:flex; justify-content:center;" target="_blank">
                        Read More at
                        <span style="text-transform:uppercase; padding-left:5px; padding-right:5px;">
                            ${url.substring(url.indexOf("sources") + 8, url.indexOf("&"))}
                        </span> Website
                    </a>
                </span>
    
            </details>
        </h3>
        <hr>`;

            document.body.innerHTML += myNews;



        }
    }
}

getNewsApi();