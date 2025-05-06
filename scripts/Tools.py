import requests
import time 
import sys

def DownloadFile(url, filepath):
    with open(filepath, 'wb') as f:
        print('Waiting for response...')
        response = requests.get(url, stream=True)
        total = response.headers.get('content-length')
        print('Downloading')
        if total is None:
            f.write(response.content)
        else:
            downloaded = 0
            total = int(total)
            startTime = time.time()
            for data in response.iter_content(chunk_size = max(int(total / 1000), 1024 * 1024)):
                downloaded += len(data)
                f.write(data)
                done = int(50 * downloaded / total)
                percentage = (downloaded / total) * 100
                elapsedTime = time.time() - startTime
                averageKBPerSecond = (downloaded / 1024) / elapsedTime
                averageSpeedString = '{:.2f} KB/s'.format(averageKBPerSecond)
                if (averageKBPerSecond > 1024):
                    averageMBPerSecond = averageKBPerSecond / 1024
                    averageSpeedString = '{:.2f} MB/s'.format(averageMBPerSecond)
                sys.stdout.write('\r[{}{}] {:.2f}% ({})     '.format('█' * done, '.' * (50 - done), percentage, averageSpeedString))
                sys.stdout.flush()
    sys.stdout.write('\n')

def Judge():
    while True:
        reply = str(input('Y/N: ')).lower().strip()
        if reply[:1] == 'y':
            return True
        if reply[:1] == 'n':
            return False
