import os
import subprocess
import Check

Check.ValidatePackages()

import Vulkan
import Tools

import colorama
from colorama import Fore
from colorama import Back
from colorama import Style

os.chdir('../')
colorama.init()

if (not Vulkan.CheckVulkanSDK()):
    print("Vulkan SDK not installed!")
    exit()

print(f"{Style.BRIGHT}{Back.GREEN}Vulkan SDK located.{Style.RESET_ALL}")
subprocess.call(["premake/premake5.exe", "vs2022"])