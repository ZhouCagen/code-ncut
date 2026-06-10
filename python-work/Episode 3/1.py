import os
import re
import math
import time
import random
import requests
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.by import By
from bs4 import BeautifulSoup
import pandas as pd

pd.set_option('display.unicode.ambiguous_as_wide', True)
pd.set_option('display.unicode.east_asian_width', True)

br = lambda: '-' * 80 + '\n'


class UniversitySpider:

    def __init__(self):
        self.driver = webdriver.Chrome()
        self.driver.get('https://www.shanghairanking.cn/rankings/bcur/2023')

        self.baseLabel = ['排名', '名称', '省市', '类型', '总分']
        self.extraLabel = [
            '办学层次', '学科水平', '办学资源', '师资规模与结构',
            '人才培养', '科学研究', '服务社会', '高端人才',
            '重大项目与成果', '国际竞争力'
        ]

        self.year = 2023

        list2dict = lambda l: {v: i for i, v in enumerate(l)}
        self.baseLabel2Idx = list2dict(self.baseLabel)
        self.extraLabel2Idx = list2dict(self.extraLabel)

        self.__toTop = lambda: self.driver.execute_script("document.documentElement.scrollTop=0")

    def __changeSort(self):
        sortBtn = self.driver.find_element(
            By.XPATH,
            r'//*[@id="content-box"]/div[2]/table/thead/tr/th[5]/div/div[2]/span'
        )
        sortBtn.click()
        sortBtn.click()

    def __changeYear(self, year):
        if self.year == year:
            return
        self.year = year
        self.yearInput = self.driver.find_element(
            By.XPATH,
            r"//div[@id='textexpansion']//input"
        )
        self.__toTop()
        self.yearInput.click()
        self.driver.implicitly_wait(10)
        options = self.driver.find_elements(
            By.XPATH,
            r"//div[@id='content']//ul[1]//li"
        )
        options[2025 - year].click()
        self.driver.implicitly_wait(10)
        time.sleep(1)
        self.__changeSort()

    def __changeCategory(self, idx):
        self.categoryInput = self.driver.find_element(
            By.XPATH,
            r"//div[@id='content-box']//tr[1]//th[6]//input"
        )
        self.__toTop()
        self.categoryInput.click()
        self.driver.implicitly_wait(10)

        options = self.driver.find_elements(
            By.XPATH,
            r'//div[@id="content-box"]/div[2]/table/thead/tr/th[6]/div/div[1]/div[2]/ul/li'
        )
        options[idx].click()
        self.driver.implicitly_wait(10)

    def __changePage(self, page):
        self.pageInput = self.driver.find_element(
            By.XPATH,
            r'//div[@id="content-box"]//div[@class="ant-pagination-options-quick-jumper"]/input'
        )
        self.pageInput.send_keys(str(page) + Keys.ENTER)
        self.driver.implicitly_wait(10)

    def __getPageInfo(self, page, num=30, getExtra=True):
        self.__changePage(page)
        line = self.driver.find_element(
            By.XPATH,
            r'//*[@id="content-box"]/div[2]/table/tbody'
        )

        soup = BeautifulSoup(line.get_attribute('outerHTML'), 'lxml')
        baseData = [item.get_text().strip().split('\n')[0] for i, item in enumerate(soup.find_all('td')) if
                    i < num * (len(self.baseLabel) + 1)]
        baseData = [[baseData[i + j] for i in range(0, len(baseData), len(self.baseLabel) + 1)] for j in
                    range(len(self.baseLabel) + 1)]
        baseData = {self.baseLabel[i]: baseData[i] for i in range(len(self.baseLabel))}

        lines = self.driver.find_elements(
            By.XPATH,
            r'//*[@id="content-box"]/div[2]/table/tbody/tr/td[6]'
        )

        if not getExtra:
            return baseData

        extraData = {}
        for i in range(len(self.extraLabel)):
            self.__changeCategory(i)
            self.__changePage(page)
            lines = self.driver.find_elements(
                By.XPATH,
                r'//*[@id="content-box"]/div[2]/table/tbody/tr/td[6]'
            )
            extraData[self.extraLabel[i]] = [
                float(line.text.strip() if re.match(r'^(([1-9]\d*)|0)(\.\d*[1-9])?$', line.text.strip()) else 0) for
                i, line in enumerate(lines) if i < num]

        return {**baseData, **extraData}

    def getInfo(self, num, year, getExtra=True):
        self.__changeYear(year)
        df = None
        for i in range(min(math.ceil(num / 30), 20)):
            data = self.__getPageInfo(i + 1, num - i * 30, getExtra)
            data = pd.DataFrame(data).set_index(self.baseLabel[0])
            df = data if df is None else pd.concat([df, data], axis=0)
        return df

    def __del__(self):
        self.driver.quit()


def getCodeInfo():
    headers = {
        'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/135.0.0.0 Safari/537.36'
    }

    def getData(url):
        response = requests.get(url, headers=headers)
        data = response.content.decode('gbk')
        data = re.findall(
            # r'stock/.*?>(.*?)\((\d*?)\)<',
            r'\">(.*?)\((\d*?)\)<',
            data
        )
        return data

    data1 = getData('http://www.cgedt.com/stockcode/hushi.asp')
    data2 = getData('http://www.cgedt.com/stockcode/shenshi.asp')

    return data1 + data2


def main():
    num = int(input("大学排名数量:"))
    num = min(num, 500)

    us = UniversitySpider()
    for year in range(2023, 2026):
        print(br() + str(year) + "年:")
        print(us.getInfo(num, year))

    schoolData = us.getInfo(num, 2025, False)
    codeData = getCodeInfo()
    indices = set()
    while len(indices) < num: indices.add(random.randint(0, len(codeData) - 1))
    # schoolData['股票'] = [codeData[i][0] for i in list(indices)]
    # schoolData['股票代码'] = [codeData[i][1] for i in list(indices)]
    for i in range(2): schoolData[['股票', '股票代码'][i]] = [codeData[j][i] for j in list(indices)]

    print(br(), schoolData)


if __name__ == "__main__":
    main()