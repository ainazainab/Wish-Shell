# How to Find SSH URL and Add SSH Key in GitLab

## Step 1: Find Your SSH URL in GitLab

After creating your project, you'll be on the project page. Here's where to find the SSH URL:

### Method 1: On the Project Home Page
1. Look at the **top of your project page**
2. You'll see a section that says **"Clone"** or **"Clone with SSH"**
3. You'll see a URL like:
   ```
   git@gitlab.imt-atlantique.fr:your-username/your-project-name.git
   ```
4. Click the **copy button** (📋 icon) next to it

### Method 2: Settings
1. Go to your project page
2. Click **Settings** (in the left sidebar)
3. Scroll down to **"Repository"** section
4. Expand **"Push to a git repository"** 
5. You'll see the SSH URL there

### Visual Guide:
```
┌─────────────────────────────────────────┐
│  Your Project Name                      │
│                                         │
│  [Clone] ▼                              │
│  ┌─────────────────────────────────┐   │
│  │ git@gitlab.imt-atlantique.fr:   │   │
│  │ username/project-name.git        │   │
│  │ [📋 Copy]                        │   │
│  └─────────────────────────────────┘   │
│                                         │
│  HTTPS URL: https://gitlab...          │
└─────────────────────────────────────────┘
```

## Step 2: Your SSH Public Key

**YOUR SSH PUBLIC KEY IS:**
```
ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIIsZ64MHtoUrmNtY8t/KPSesgjqKdyyspk/napLws2C6 wish-shell-project
```

**Copy the ENTIRE line above** (starting with `ssh-ed25519` and ending with `wish-shell-project`)

## Step 3: Add SSH Key to GitLab

1. **Go to your SSH Keys page:**
   - Direct link: https://gitlab.imt-atlantique.fr/-/profile/keys
   - Or: Click your **profile picture** (top right) → **Preferences** → **SSH Keys** (in left menu)

2. **On the SSH Keys page:**
   - Click **"Add new key"** button
   - **Title:** Enter something like "WSL Key" or "My Laptop"
   - **Key:** Paste your SSH public key (the entire line from Step 2)
   - Click **"Add key"**

3. **Verify it worked:**
   - You should see your key listed on the page
   - It should show "Usage: Authentication"

## Step 4: Test Your SSH Connection

In WSL, run:
```bash
ssh -T git@gitlab.imt-atlantique.fr
```

You should see:
```
Welcome to GitLab, @your-username!
```

## Step 5: Connect Your Project to GitLab

Once you have:
- ✅ Your SSH URL (from Step 1)
- ✅ SSH key added to GitLab (Step 3)

Run in WSL:
```bash
cd "/mnt/c/Users/lenovo/OS project/processes-shell"
git remote add origin git@gitlab.imt-atlantique.fr:YOUR_USERNAME/YOUR_PROJECT_NAME.git
```

Replace `YOUR_USERNAME/YOUR_PROJECT_NAME` with what you see in your SSH URL!

## Troubleshooting

**Can't find the Clone button?**
- Make sure you're on the main project page (not Settings)
- Look for a blue **"Clone"** button near the top

**SSH URL format:**
- Should start with: `git@gitlab.imt-atlantique.fr:`
- Should end with: `.git`
- Example: `git@gitlab.imt-atlantique.fr:john.doe/simple-unix-shell.git`

**"Permission denied" when testing?**
- Make sure you copied the ENTIRE key (one long line)
- Wait a minute after adding the key, then try again

