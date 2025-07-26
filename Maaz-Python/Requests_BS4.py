import requests
from bs4 import BeautifulSoup
import time

res = requests.get("https://www.euronews.com/")
soup = BeautifulSoup(res.text, "html.parser")

headlines = soup.find_all("h2", class_="the-media-object__title")

print("\nStatus code: ", res.status_code)
print("\n")

all_news = []

for headline in soup.find_all("h2", class_="the-media-object__title"):
    main_news = (
        "" if headline.find_parent("a") is None else headline.find_parent("a")["href"]
    )
    all_news.append(
        {
            "head": headline.get_text(strip=True),
            "content": "",
            "main-news": main_news,
        }
    )

for obj in all_news:
    print("Fetching site:", "https://euronews.com" + str(obj["main-news"]))
    # req = requests.get("https://euronews.com" + str(obj["main-news"]))
    req = requests.get(
        "https://euronews.com" + str(obj["main-news"]),
        headers={"User-Agent": "Mozilla/5.0"},
    )
    time.sleep(0.5)
    req_soup = BeautifulSoup(req.text, "html.parser")
    article_div = req_soup.find("div", class_="c-article-content")
    if article_div:
        for p in req_soup.select("div.c-article-content > p"):
            # for p in article_div.find_all("p"):
            obj["content"] += "<p>" + p.get_text(strip=True) + "</p>"

print("\n")

overall_news = ""

for obj in all_news:
    overall_news += (
        '<li><h2 style="display:inline-block">'
        + obj["head"]
        + "</h2>"
        + "<br><div>"
        + obj["content"]
        + "</div></li><br><br>"
    )

html_body = f'<!DOCTYPE html>\
<html lang="en">\
<head>\
    <meta charset="UTF-8">\
    <meta name="viewport" content="width=device-width, initial-scale=1.0">\
    <title>Document</title>\
</head>\
<body>\
<ul>\
{overall_news}\
</ul>\
</body>\
</html>'

with open(
    "C:\\Users\\Admin\\AppData\\Local\\Temp\\temp_Scraping_Euro_News.html", "w"
) as f:

    f.write(overall_news)
    print("News uploaded succesfully!!!\n")
