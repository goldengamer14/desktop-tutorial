import pandas as pd
import numpy as np
import requests


def Generate_Results(numbers):
    response = requests.get(f"https://randomuser.me/api/?results={numbers}")
    people = response.json()["results"]

    names = []
    genders = []
    for person in people:
        names.append(f"{person['name']['first']}")
        genders.append(person["gender"].capitalize())  # "male" → "Male"

    categories = ["General", "OBC", "SC", "ST"]
    category_values = [
        np.random.choice(categories, p=[0.6, 0.2, 0.1, 0.1]) for _ in range(len(names))
    ]

    percentiles = np.round(np.random.uniform(40 / 100, 99.99 / 100, size=len(names)), 4)

    data_table = pd.DataFrame(
        {
            "Name": names,
            "Gender": genders,
            "Category": category_values,
            "Percentile": percentiles,
        }
    )

    print("Data Table generated:\n", data_table)
    return data_table


def Generate_Finance_List(numbers):
    people = requests.get(f"https://randomuser.me/api/?results={numbers}").json()[
        "results"
    ]

    names = []
    for person in people:
        names.append(person["name"]["first"])

    incomes = np.random.randint(0, 26, len(names))
    incomes = [x * 10**5 for x in incomes]
    taxable_percent = taxable_amount = pd.Series(np.zeros(shape=(len(names))))

    data_table = pd.DataFrame(
        {
            "Name": names,
            "Income": incomes,
            "Taxable Percent": taxable_percent,
            "Taxable Amount": taxable_amount,
        }
    )

    print("Data Table generated:\n", data_table)
    return data_table


def Generate_Dates(numbers):
    names = []
    for person in requests.get(f"https://randomuser.me/api/?results={numbers}").json()[
        "results"
    ]:
        names.append(person["name"]["first"])

    years = np.random.randint(1991, 2026, size=numbers)
    months = np.random.randint(1, 13, size=numbers)
    dates = np.random.randint(1, 32, size=numbers)

    months_31 = [1, 3, 5, 7, 8, 10, 12]
    for i in range(0, numbers):
        if dates[i] > 28 and months[i] == 2:
            dates[i] = np.random.randint(0, 29)
        elif dates[i] > 30 and not months[i] in months_31:
            dates[i] = np.random.randint(0, 31)

    data_table = pd.DataFrame(
        {
            "Date": dates,
            "Month": months,
            "Year": years,
            "Whole Date": np.zeros(shape=numbers),
        }
    )

    print("Data Table generated:\n", data_table)
    return data_table


def Generate_Names_With_Nationality(numbers):
    nat_to_country = {
        "IN": "Indian",
        "PK": "Pakistani",
        "US": "American",
        "GB": "British",
        "FR": "French",
        "DE": "German",
        "BR": "Brazilian",
        "CA": "Canadian",
        "AU": "Australian",
        "ES": "Spanish",
    }

    nations = ",".join([x for x in nat_to_country.keys()])

    names = []
    nationalities = []
    for person in requests.get(
        f"https://randomuser.me/api/?results={numbers}&nat={nations}"
    ).json()["results"]:
        name = person["name"]["first"]
        nationality = nat_to_country.get(person["nat"], "Non-national")
        if nationality != "Non-national":
            names.append(name)
            nationalities.append(nationality)

    data_table = pd.DataFrame(
        {
            "Name": names,
            "Nationality": nationalities,
            "Foreigner": ["#VALUE" for i in range(0, len(names))],
        }
    )

    print("Data Table generated:\n", data_table)
    return data_table


def Generate_PCM_Marks(numbers):
    names = []
    genders = []
    for person in requests.get(
        f"https://randomuser.me/api/?results={numbers}&nat=IN"
    ).json()["results"]:
        names.append(person["name"]["first"])
        genders.append(person["gender"].capitalize())

    # Sn = n/2(2a + (n-1)d); 1 = 40/2(39d); d = 1 / 20 / 39;
    prob = np.array([np.sin(np.radians(i * 2 / 90 * 40)) for i in range(0, 40)])

    Physics_marks = np.random.choice(
        np.arange(10, 50),
        # p=[i / 20 / 39 for i in range(0, 40)],
        p=[prob[i] / prob.sum() for i in range(0, len(prob))],
        size=numbers,
        replace=True,
    )

    prob = np.array([np.sin(np.radians(i * 2 / 90 * 35)) for i in range(0, 35)])

    Chemistry_marks = np.random.choice(
        np.arange(15, 50),
        # p=[i / 35 / 17 for i in range(0, 35)],
        p=[prob[i] / prob.sum() for i in range(0, len(prob))],
        size=numbers,
        replace=True,
    )

    prob = np.array([np.sin(np.radians(i * 2 / 90 * 90)) for i in range(0, 90)])

    Maths_marks = np.random.choice(
        np.arange(10, 100),
        # p=[i / 45 / 89 for i in range(0, 90)],
        p=[prob[i] / prob.sum() for i in range(0, len(prob))],
        size=numbers,
        replace=True,
    )

    data_table = pd.DataFrame(
        {
            "Name": names,
            "Gender": genders,
            "Physics": Physics_marks,
            "Chemistry": Chemistry_marks,
            "Maths": Maths_marks,
            "Total": pd.Series(
                [
                    Physics_marks[i] + Chemistry_marks[i] + Maths_marks[i]
                    for i in range(0, numbers)
                ]
            ),
        }
    )

    print(data_table)
    return data_table


# data_table = Generate_Results(20)
# data_table = Generate_Finance_List(15)
# data_table = Generate_Dates(20)
# data_table = Generate_Names_With_Nationality(30)
# data_table = Generate_PCM_Marks(10)
data_table = Generate_PCM_Marks(100)
data_table_csv = Generate_PCM_Marks(100)

# Export to Excel
data_table.to_excel("Maaz-Python\\Data_Generator\\Generated_Data.xlsx", index=False)
data_table_csv.to_csv("Maaz-Python\\Data_Generator\\Generated_Data.csv", index=False)

"""
    # Sn[0->89](sin(n)) = 1;
    p = np.array([np.sin(np.radians(i * 2)) for i in range(0, 90)])
    p = np.array([x / p.sum() for x in p])

    # print(p, p.sum())
    median_arr = np.zeros(shape=20)
    for i in range(20):
        median_arr[i] = np.median(np.random.choice(np.arange(0, 90), p=p, size=100))

    print(median_arr)
    print("Median of Medians: ", np.median(np.median(median_arr)))
"""
