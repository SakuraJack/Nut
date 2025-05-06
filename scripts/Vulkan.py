import os
import subprocess
import sys
from pathlib import Path
import Tools
from io import BytesIO
import colorama
from colorama import Fore
from colorama import Back
from colorama import Style

VULKAN_SDK = os.getenv("Vulkan_SDK")
REQUIRED_VULKAN_VERSION = '1.3.'
INSTALL_VULKAN_VERSION = '1.4.309.0'
VULKAN_SDK_INSTALLER_URL = f'https://sdk.lunarg.com/sdk/download/{INSTALL_VULKAN_VERSION}/windows/VulkanSDK-{INSTALL_VULKAN_VERSION}-Installer.exe'
VULKAN_SDK_LOCAL_PATH = 'Nut/vendor/VulkanSDK'
VULKAN_SDK_EXE_PATH = f'{VULKAN_SDK_LOCAL_PATH}/VulkanSDK.exe'

colorama.init()

def InstallVulkanSDK():
    # 安装Vulkan SDK
    Path(VULKAN_SDK_LOCAL_PATH).mkdir(parents=True, exist_ok=True)
    print('Downloading {} to {}'.format(VULKAN_SDK_INSTALLER_URL, VULKAN_SDK_EXE_PATH))
    Tools.DownloadFile(VULKAN_SDK_INSTALLER_URL, VULKAN_SDK_EXE_PATH)
    print("Running Vulkan SDK installer...")
    print(f"{Style.BRIGHT}{Back.YELLOW}Make sure to install shader debug libs if you want to build in Debug!")
    os.startfile(os.path.abspath(VULKAN_SDK_EXE_PATH))
    print(f"{Style.BRIGHT}{Back.RED}Re-run this script after installation{Style.RESET_ALL}")

def InstallVulkanPrompt():
    # 是否选择安装Vulkan SDK
    print("Would you like to install the Vulkan SDK?")
    install = Tools.Judge()
    if (install):
        InstallVulkanSDK()
        quit()

def CheckVulkanSDK():
    if(VULKAN_SDK is None):
        print(f"{Style.BRIGHT}{Back.RED}You don't have the Vulkan SDK installed!{Style.RESET_ALL}")
        InstallVulkanPrompt()
        return False
    elif (REQUIRED_VULKAN_VERSION not in VULKAN_SDK):
        print(f"Located Vulkan SDK at {VULKAN_SDK}")
        print(f"{Style.BRIGHT}{Back.RED}You don't have the correct Vulkan SDK verstion! (Required {REQUIRED_VULKAN_VERSION}){Style.RESET_ALL}")
        InstallVulkanPrompt()
        return False
    
    print(f"{Style.BRIGHT}{Back.GREEN}Correct Vulkan SDK located at {VULKAN_SDK}{Style.RESET_ALL}")
    return True