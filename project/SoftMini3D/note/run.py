import re
import requests
import os
from urllib.parse import urljoin

def download_images(md_file_path, local_dir):
    with open(md_file_path, 'r',encoding='UTF-8') as f:
        md_content = f.read()

    image_urls = re.findall('!\[.*?\]\((.*?)\)', md_content)
    local_urls = []
    
    if not os.path.exists(local_dir):
        os.makedirs(local_dir)

    for url in image_urls:
        img_data = requests.get(url).content
        # 使用re.sub去掉文件名中的非字母、数字和下划线字符
        clean_filename = re.sub('\W+','', os.path.basename(url)) + '.jpg'
        # clean_filename = re.search(r'\d{17}', url).group(0) + '.jpg'
        print(clean_filename)
        local_file = os.path.join(local_dir, clean_filename)
        with open(local_file, 'wb') as handler:
            handler.write(img_data)
        local_urls.append(local_file)

    for original_url, local_url in zip(image_urls, local_urls):
        md_content = md_content.replace(original_url, local_url)

    with open(md_file_path, 'w',encoding='UTF-8') as f:
        f.write(md_content)

# 使用方法：download_images('path/to/your/markdown.md', 'path/to/local/directory')

if __name__ == "__main__":
	# 将myFile.md中的超链接批量下载到images文件夹中
	# 并修改myFile.md中超链接图片，使其指向本地文件
    download_images('1.md', './jpg/')
