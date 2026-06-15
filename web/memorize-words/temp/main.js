// 全局变量
let currentBook = null;
let currentMode = null;
let words = [];
let currentWord = null;
let practiceOrder = [];

// DOM元素
const bookListEl = document.getElementById('book-list');
const newBookNameEl = document.getElementById('new-book-name');
const createBookBtn = document.getElementById('create-book');
const deleteBookBtn = document.getElementById('delete-book');
const newWordEl = document.getElementById('new-word');
const newTranslationEl = document.getElementById('new-translation');
const addSingleBtn = document.getElementById('add-single');
const csvFileEl = document.getElementById('csv-file');
const importCsvBtn = document.getElementById('import-csv');
const modeButtons = document.querySelectorAll('.mode-selector button');
const practiceArea = document.getElementById('practice-area');
const nextWordBtn = document.getElementById('next-word');
const markFamiliarBtn = document.getElementById('mark-familiar');

// 初始化应用
async function init() {
    // 加载单词本列表
    await loadBookList();
    // 绑定事件
    setupEventListeners();
}

// 事件监听器设置
function setupEventListeners() {
    createBookBtn.addEventListener('click', createNewBook);
    deleteBookBtn.addEventListener('click', deleteCurrentBook);
    bookListEl.addEventListener('change', bookSelected);
    addSingleBtn.addEventListener('click', addSingleWord);
    importCsvBtn.addEventListener('click', importCsv);
    
    modeButtons.forEach(btn => {
        btn.addEventListener('click', () => {
            const mode = btn.dataset.mode;
            startPracticeMode(mode);
        });
    });
    
    nextWordBtn.addEventListener('click', showNextWord);
    markFamiliarBtn.addEventListener('click', markWordAsFamiliar);
}

// 加载单词本列表
async function loadBookList() {
    try {
        const response = await fetch('/books');
        const books = await response.json();
        
        bookListEl.innerHTML = '';
        books.forEach(book => {
            const option = document.createElement('option');
            option.value = book.id;
            option.textContent = book.name;
            bookListEl.appendChild(option);
        });
        
        if (books.length > 0) {
            currentBook = books[0].id;
            bookListEl.value = currentBook;
        }
    } catch (error) {
        console.error('加载单词本失败:', error);
    }
}

// 创建新单词本
async function createNewBook() {
    const name = newBookNameEl.value.trim();
    if (!name) return;
    
    try {
        const response = await fetch('/books', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ name })
        });
        
        if (response.ok) {
            await loadBookList();
            newBookNameEl.value = '';
        }
    } catch (error) {
        console.error('创建单词本失败:', error);
    }
}

// 删除单词本
async function deleteCurrentBook() {
    if (!currentBook) return;
    
    try {
        const response = await fetch(`/books/${currentBook}`, {
            method: 'DELETE'
        });
        
        if (response.ok) {
            await loadBookList();
        }
    } catch (error) {
        console.error('删除单词本失败:', error);
    }
}

// 单词本选择变更
async function bookSelected(event) {
    currentBook = event.target.value;
    await loadWordsForBook();
}

// 加载当前单词本的单词
async function loadWordsForBook() {
    if (!currentBook) return;
    
    try {
        const response = await fetch(`/books/${currentBook}/words`);
        words = await response.json();
    } catch (error) {
        console.error('加载单词失败:', error);
        words = [];
    }
}

// 添加单个单词
async function addSingleWord() {
    const word = newWordEl.value.trim();
    const translation = newTranslationEl.value.trim();
    
    if (!word || !translation || !currentBook) return;
    
    try {
        const response = await fetch(`/books/${currentBook}/words`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ word, translation })
        });
        
        if (response.ok) {
            await loadWordsForBook();
            newWordEl.value = '';
            newTranslationEl.value = '';
        }
    } catch (error) {
        console.error('添加单词失败:', error);
    }
}

// 导入CSV
async function importCsv() {
    if (!csvFileEl.files.length || !currentBook) return;
    
    const formData = new FormData();
    formData.append('file', csvFileEl.files[0]);
    
    try {
        const response = await fetch(`/books/${currentBook}/import`, {
            method: 'POST',
            body: formData
        });
        
        if (response.ok) {
            await loadWordsForBook();
            csvFileEl.value = '';
        }
    } catch (error) {
        console.error('导入CSV失败:', error);
    }
}

// 开始学习模式
async function startPracticeMode(mode) {
    if (!currentBook || words.length === 0) return;
    
    // 加载单词
    await loadWordsForBook();
    
    // 设置当前模式
    currentMode = mode;
    
    // 打乱单词顺序
    practiceOrder = [...words.filter(word => !word.familiar)];
    shuffleArray(practiceOrder);
    
    // 开始第一个单词
    showNextWord();
}

// 显示下一个单词
function showNextWord() {
    if (practiceOrder.length === 0) {
        practiceArea.innerHTML = "<p>没有更多单词需要练习了!</p>";
        return;
    }
    
    // 获取下一个单词
    currentWord = practiceOrder.pop();
    
    // 根据模式显示内容
    switch (currentMode) {
        case 'spell':
            showSpellingPractice();
            break;
        case 'choice':
            showChoicePractice();
            break;
        case 'recall':
            showRecallPractice();
            break;
        case 'listen':
            showListenPractice();
            break;
    }
}

// 拼写练习模式
function showSpellingPractice() {
    practiceArea.innerHTML = `
        <h3>${currentWord.translation}</h3>
        <p>请输入英文拼写：</p>
        <input type="text" id="spell-input" placeholder="输入英文单词">
        <button id="check-spell">检查</button>
    `;
    
    document.getElementById('check-spell').addEventListener('click', () => {
        const input = document.getElementById('spell-input').value.trim().toLowerCase();
        const result = input === currentWord.word.toLowerCase();
        
        practiceArea.innerHTML += `
            <p class="${result ? 'correct' : 'incorrect'}">
                ${result ? '✓ 正确！' : '✗ 错误：' + currentWord.word}
            </p>
        `;
    });
}

// 四选一模式
function showChoicePractice() {
    // 获取3个随机错误选项
    const otherWords = words
        .filter(w => w.id !== currentWord.id && !w.familiar)
        .sort(() => Math.random() - 0.5)
        .slice(0, 3)
        .map(w => w.translation);
    
    const allOptions = [currentWord.translation, ...otherWords];
    shuffleArray(allOptions);
    
    practiceArea.innerHTML = `
        <h3>${currentWord.word}</h3>
        <p>请选择正确的中文翻译：</p>
        <div class="choices">
            ${allOptions.map((opt, i) => `
                <button class="choice-btn" data-option="${opt}">${opt}</button>
            `).join('')}
        </div>
    `;
    
    document.querySelectorAll('.choice-btn').forEach(btn => {
        btn.addEventListener('click', () => {
            const isCorrect = btn.dataset.option === currentWord.translation;
            btn.classList.add(isCorrect ? 'correct' : 'incorrect');
            
            if (!isCorrect) {
                // 高亮显示正确选项
                document.querySelectorAll('.choice-btn').forEach(b => {
                    if (b.dataset.option === currentWord.translation) {
                        b.classList.add('correct');
                    }
                });
            }
        });
    });
}

// 快速回忆模式
function showRecallPractice() {
    practiceArea.innerHTML = `
        <h3>${currentWord.word}</h3>
        <p>你认识这个单词吗？</p>
        <div>
            <button id="btn-remember">认识</button>
            <button id="btn-forget">不认识</button>
        </div>
        <div id="translation-result" class="hidden">
            <p>翻译: ${currentWord.translation}</p>
        </div>
    `;
    
    document.getElementById('btn-remember').addEventListener('click', () => {
        // 继续下一个单词
        showNextWord();
    });
    
    document.getElementById('btn-forget').addEventListener('click', () => {
        document.getElementById('translation-result').classList.remove('hidden');
    });
}

// 听写练习模式
function showListenPractice() {
    practiceArea.innerHTML = `
        <h3>听写练习</h3>
        <p>${currentWord.translation}</p>
        <p>请拼写英文单词：</p>
        <input type="text" id="dictation-input" placeholder="输入英文单词">
        <button id="check-dictation">检查</button>
        <button id="listen-again">再听一次</button>
    `;
    
    // 朗读单词
    speakWord(currentWord.word);
    
    document.getElementById('check-dictation').addEventListener('click', () => {
        const input = document.getElementById('dictation-input').value.trim().toLowerCase();
        const result = input === currentWord.word.toLowerCase();
        
        practiceArea.innerHTML += `
            <p class="${result ? 'correct' : 'incorrect'}">
                ${result ? '✓ 正确！' : '✗ 错误：' + currentWord.word}
            </p>
        `;
    });
    
    document.getElementById('listen-again').addEventListener('click', () => {
        speakWord(currentWord.word);
    });
}

// 标记单词为"熟"
async function markWordAsFamiliar() {
    if (!currentBook || !currentWord) return;
    
    try {
        const response = await fetch(`/books/${currentBook}/words/${currentWord.id}/familiar`, {
            method: 'PUT'
        });
        
        if (response.ok) {
            // 从练习队列中移除
            practiceOrder = practiceOrder.filter(w => w.id !== currentWord.id);
            
            // 显示下一个单词
            showNextWord();
        }
    } catch (error) {
        console.error('标记单词失败:', error);
    }
}

// 工具函数：随机打乱数组
function shuffleArray(array) {
    for (let i = array.length - 1; i > 0; i--) {
        const j = Math.floor(Math.random() * (i + 1));
        [array[i], array[j]] = [array[j], array[i]];
    }
}

// 工具函数：单词朗读
function speakWord(text) {
    if ('speechSynthesis' in window) {
        const utterance = new SpeechSynthesisUtterance(text);
        utterance.lang = 'en-US';
        speechSynthesis.speak(utterance);
    }
}

// 启动应用
document.addEventListener('DOMContentLoaded', init);
