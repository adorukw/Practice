const express = require('express');
const fs = require('fs');  // 标准fs模块，包含流处理API
const fsp = fs.promises;
const path = require('path');
const csvParser = require('csv-parser');
const multer = require('multer');

const app = express();
const port = 3000;
const DATA_DIR = './data';

// 确保数据目录存在
fsp.mkdir(DATA_DIR, { recursive: true }).catch(console.error);

// 中间件
app.use(express.json());
app.use(express.static('public'));

// 文件上传处理
const upload = multer({ dest: 'uploads/' });

// 获取所有单词本
app.get('/books', async (req, res) => {
    try {
        const books = await getBookList();
        res.json(books);
    } catch (error) {
        res.status(500).json({ error: '获取单词本失败' });
    }
});

// 创建新单词本
app.post('/books', async (req, res) => {
    const { name } = req.body;
    if (!name) return res.status(400).json({ error: '名称不能为空' });
    
    try {
        const books = await getBookList();
        const newBook = {
            id: Date.now().toString(),
            name,
            createdAt: new Date().toISOString()
        };
        
        books.push(newBook);
        await saveBookList(books);
        res.status(201).json(newBook);
    } catch (error) {
        res.status(500).json({ error: '创建单词本失败' });
    }
});

// 删除单词本
app.delete('/books/:id', async (req, res) => {
    const bookId = req.params.id;
    
    try {
        let books = await getBookList();
        books = books.filter(book => book.id !== bookId);
        await saveBookList(books);
        
        // 删除关联的单词文件
        const wordFile = path.join(DATA_DIR, `words_${bookId}.json`);
        await fsp.unlink(wordFile).catch(() => {});
        
        res.status(204).send();
    } catch (error) {
        res.status(500).json({ error: '删除单词本失败' });
    }
});

// 获取单词本中的单词
app.get('/books/:bookId/words', async (req, res) => {
    const bookId = req.params.bookId;
    
    try {
        const words = await getWordsForBook(bookId);
        res.json(words);
    } catch (error) {
        res.status(500).json({ error: '获取单词失败' });
    }
});

// 添加单词到单词本
app.post('/books/:bookId/words', async (req, res) => {
    const bookId = req.params.bookId;
    const { word, translation } = req.body;
    
    if (!word || !translation) {
        return res.status(400).json({ error: '单词和翻译不能为空' });
    }
    
    try {
        const words = await getWordsForBook(bookId);
        const newWord = {
            id: Date.now().toString(),
            word: word.trim(),
            translation: translation.trim(),
            familiar: false,
            createdAt: new Date().toISOString()
        };
        
        words.push(newWord);
        await saveWordsForBook(bookId, words);
        res.status(201).json(newWord);
    } catch (error) {
        res.status(500).json({ error: '添加单词失败' });
    }
});

// 标记单词为"熟"
app.put('/books/:bookId/words/:wordId/familiar', async (req, res) => {
    const { bookId, wordId } = req.params;
    
    try {
        const words = await getWordsForBook(bookId);
        const wordIndex = words.findIndex(w => w.id === wordId);
        
        if (wordIndex === -1) {
            return res.status(404).json({ error: '单词未找到' });
        }
        
        words[wordIndex].familiar = true;
        await saveWordsForBook(bookId, words);
        res.json(words[wordIndex]);
    } catch (error) {
        res.status(500).json({ error: '标记单词失败' });
    }
});

// 导入CSV到单词本
app.post('/books/:bookId/import', upload.single('file'), async (req, res) => {
    const bookId = req.params.bookId;
    const filePath = req.file?.path; // 添加可选链防止undefined
    
    console.log(`开始导入CSV到单词本 ${bookId}, 文件路径: ${filePath}`); // 添加日志
    
    if (!filePath || !bookId) {
        return res.status(400).json({ error: '无效请求' });
    }
    
    try {
        const words = await getWordsForBook(bookId);
        const newWords = [];
        
        // 解析CSV文件
        await new Promise((resolve, reject) => {
            fs.createReadStream(filePath)
                .pipe(csvParser())
                .on('data', (row) => {
                    const word = row['word'] || row['Word'] || Object.values(row)[0];
                    const translation = row['translation'] || row['Translation'] || Object.values(row)[1];
                    
                    if (word && translation) {
                        newWords.push({
                            id: Date.now().toString() + Math.random(),
                            word: word.trim(),
                            translation: translation.trim(),
                            familiar: false,
                            createdAt: new Date().toISOString()
                        });
                    }
                })
                .on('end', resolve)
                .on('error', reject);
        });
        
        // 删除临时文件
        await fsp.unlink(filePath);
        
        // 保存新单词
        const updatedWords = [...words, ...newWords];
        await saveWordsForBook(bookId, updatedWords);
        
        res.status(201).json({ count: newWords.length });
    } catch (error) {
        console.error('导入失败:', {
            message: error.message,
            stack: error.stack,
            bookId,
            filePath
        });
        res.status(500).json({ error: '导入失败: ' + error.message });
    }
});

// 工具函数：获取单词本列表
async function getBookList() {
    const filePath = path.join(DATA_DIR, 'books.json');
    
    try {
        const data = await fsp.readFile(filePath, 'utf8');
        return JSON.parse(data);
    } catch (error) {
        // 文件不存在时返回空数组
        return [];
    }
}

// 工具函数：保存单词本列表
async function saveBookList(books) {
    const filePath = path.join(DATA_DIR, 'books.json');
    await fsp.writeFile(filePath, JSON.stringify(books, null, 2));
}

// 工具函数：获取指定单词本的单词
async function getWordsForBook(bookId) {
    const filePath = path.join(DATA_DIR, `words_${bookId}.json`);
    
    try {
        const data = await fsp.readFile(filePath, 'utf8');
        return JSON.parse(data);
    } catch (error) {
        // 文件不存在时返回空数组
        return [];
    }
}

// 工具函数：保存单词到单词本
async function saveWordsForBook(bookId, words) {
    const filePath = path.join(DATA_DIR, `words_${bookId}.json`);
    await fsp.writeFile(filePath, JSON.stringify(words, null, 2));
}

// 启动服务器
app.listen(port, () => {
    console.log(`服务器运行在 http://localhost:${port}`);
});
