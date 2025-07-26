import requests
from bs4 import BeautifulSoup

base_url = "https://wesmckinney.com/book/"
chapter_paths = [
    "preface.html", "intro.html", "python-basics.html",
    "ipython.html", "numpy.html", "pandas.html",
    "data-cleaning.html", "plotting.html", "data-wrangling.html",
    "aggregate.html", "time-series.html", "advanced-pandas.html",
    "data-analysis-examples.html", "appendix.html"
]

book_text = ""

for path in chapter_paths:
    url = base_url + path
    print(f"Fetching: {url}")
    res = requests.get(url)
    soup = BeautifulSoup(res.text, "html.parser")

    # Find the article content (this structure is consistent)
    content = soup.find("main")
    if content:
        text = content.get_text(separator="\n", strip=True)
        chapter_title = path.replace(".html", "").replace("-", " ").title()
        book_text += f"\n\n===== {chapter_title} =====\n\n{text}\n"

# Save to a text file
with open("Python_for_Data_Analysis.txt", "w", encoding="utf-8") as f:
    f.write(book_text)

print("✅ Book scraped and saved successfully.")
