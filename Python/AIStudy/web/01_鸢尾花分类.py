from flask import Flask, request, jsonify, render_template
import joblib
import numpy as np
import os

app = Flask(__name__)

# 加载模型
# 获取项目根目录（AI_STUDY/）
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# 加载模型
model = joblib.load(os.path.join(BASE_DIR, 'models', 'iris_classifier.pkl'))
scaler = joblib.load(os.path.join(BASE_DIR, 'models', 'iris_scaler.pkl'))
class_names = ['setosa', 'versicolor', 'virginica']

@app.route('/')
def home():
    """显示主页"""
    return '''
    <html>
        <body>
            <h2>🌺 鸢尾花分类器</h2>
            <form action="/predict" method="post">
                花萼长度(cm): <input type="text" name="sepal_length"><br>
                花萼宽度(cm): <input type="text" name="sepal_width"><br>
                花瓣长度(cm): <input type="text" name="petal_length"><br>
                花瓣宽度(cm): <input type="text" name="petal_width"><br>
                <input type="submit" value="预测">
            </form>
        </body>
    </html>
    '''

@app.route('/predict', methods=['POST'])
def predict():
    """API预测接口"""
    try:
        # 获取数据
        features = [
            float(request.form['sepal_length']),
            float(request.form['sepal_width']),
            float(request.form['petal_length']),
            float(request.form['petal_width'])
        ]
        
        # 预测
        features_array = np.array([features])
        features_scaled = scaler.transform(features_array)
        
        prediction = model.predict(features_scaled)[0]
        probabilities = model.predict_proba(features_scaled)[0]
        
        result = {
            'prediction': class_names[prediction],
            'probabilities': {
                class_names[i]: float(probabilities[i])
                for i in range(3)
            }
        }
        
        # 返回HTML结果
        return f'''
        <html>
            <body>
                <h2>预测结果: {class_names[prediction]}</h2>
                <h3>各类别概率:</h3>
                <ul>
                    <li>setosa: {probabilities[0]:.2%}</li>
                    <li>versicolor: {probabilities[1]:.2%}</li>
                    <li>virginica: {probabilities[2]:.2%}</li>
                </ul>
                <a href="/">返回</a>
            </body>
        </html>
        '''
        
    except Exception as e:
        return f"错误: {str(e)}"

@app.route('/api/predict', methods=['POST'])
def api_predict():
    """JSON API接口"""
    data = request.get_json()
    try:
        features = [
            data['sepal_length'],
            data['sepal_width'],
            data['petal_length'],
            data['petal_width']
        ]
        
        features_array = np.array([features])
        features_scaled = scaler.transform(features_array)
        
        prediction = model.predict(features_scaled)[0]
        probabilities = model.predict_proba(features_scaled)[0]
        
        return jsonify({
            'success': True,
            'prediction': class_names[prediction],
            'prediction_id': int(prediction),
            'probabilities': {
                class_names[i]: float(probabilities[i])
                for i in range(3)
            }
        })
    except Exception as e:
        return jsonify({'success': False, 'error': str(e)})

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)