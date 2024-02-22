from flask import Flask
from flask import render_template
from flask import jsonify
from datetime import datetime
from UsageTimePredict import UsageTimePredict

app = Flask(__name__)
last_movement = datetime(year=2020, month=1, day=1, hour=0, minute=0, second=0)
usageTimePredict = UsageTimePredict()

lux = 0.0
lux_threshold = 100
hum = 0.0
tmp = 0.0
tmp_ideal = 23
wsm = 'off'
wsm_threshold = 10

@app.route("/")
def index():
    return render_template('index.html')

@app.route('/get_data')
def get_data():
    global lux
    global hum
    global tmp
    global tmp_ideal
    global wsm
    global last_movement

    # Read data from the text file
    with open('data.txt', 'r') as file:
        data = file.read().split(';')

        if (int(data[1].split(':')[1]) >= lux_threshold):
            lux = usageTimePredict.update(True)

        else:
            lux = usageTimePredict.update(False)

        hum = data[2].split(':')[1] + ' %'

        tmp = data[3].split(':')[1]
        tmp_float = float(tmp)
        tmp2 = tmp_float + (tmp_ideal - tmp_float) / 2
        tmp2 = tmp2.__str__() + ' °C'
        tmp = tmp + ' °C'

        if abs(float(data[4].split(':')[1])) > .1 and abs(float(data[5].split(':')[1])) > .1:
            last_movement = datetime.now()
            wsm = 'on'

        elif (datetime.now() - last_movement).seconds >= wsm_threshold:
            wsm = 'off'

        

    return jsonify({'lux': lux, 'hum': hum, 'tmp': tmp, 'tmp2': tmp2, 'wsm': wsm})

if __name__ == '__main__':
    app.run(port=80, debug=True)