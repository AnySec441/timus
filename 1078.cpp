#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct segment {
    int left, right, id;
};

bool compare_by_lenght(const segment x, const segment y);

vector<segment> segments(1001);
int depth[501], parent[501], answer[501]; // depth - уровень вложенности

int main() {
    int i = 1, N, start, end, id_of_deepest = 1, deep_of_deepest = 1;
    cin >> N;
    while (i <= N) {
        cin >> start >> end;
        if (start > end)
            swap(start, end);
        segments[i].left = start;
        segments[i].right = end;
        segments[i].id = i;
        i++;
    }

    sort(segments.begin() + 1, segments.begin() + 1 + N, compare_by_lenght); // Сортируем по возрастанию длины, т.к более длинные очевидно не могут быть вложены
    depth[0] = 0; // т.к индексы отрезков начинается у нас с единица
    depth[1] = 1; // самый маленький отрезок получает вложенность 1 по умолчанию
    for (int i = 2; i <= N; i++) {
        depth[i] = 1; // на старте все отрезки имеют вложенность один(ну он вложен сам в себя вуаля
        for (int j = 1; j < i; j++) { // Проверяем все отрезки с меньшей длинной, вдруг вложены?
            if ((segments[i].left < segments[j].left) && (segments[j].right < segments[i].right) && // понятно, буквальная вложенность по границам
                (depth[j] + 1 > depth[i])) { /* вот это выполняется на первых итерациях, где depth[i]=depth[j]=1 => в начале мы сразу определяем кто куда вложен и вложенному даём +1
 * если мы встретим дальше отрезки типа [-10;10] [-5;5] [6;7] [-2;2] то у первого будет depth 1 у второго и третьего 2 а у последнего 3, при сравнениях на 36 строке +1 важно для того чтобы
 * те которые сейчас на одних уровнях, но они вложены(да, такое возможно) условие сработало корректно.
 * т.е на данных [-10;10] [-5;5] [6;7] [-2;2] сначала -2 2 получает depth=1  потом 6 7 получает единицу потом -5 5 единицу, потом -5 5 получает 2 при сравнении с -2 2 засчёт +1 с равными depth потом -10 10 получает единицу
 * потом -10 10 получает depth=2 засчёт сравнения с -2 2 потом он же получает depth=3 засчёт сравнения с -5 5
 * получается что -2 2; -5 5 ;-10 10 имеют depth 1 2 3 соответственно, deepest = 3 а айдишник у него 4, т.е дальше мы раскрутим от -10 10 к -5 5 и к -2 2 и к 0 0(id = 0) и завершим цикл
 */
                parent[i] = j; // присваем родителя по вложенности, чтобы можно было раскрутить дерево вложенности из листа к корню
                depth[i] = 1 + depth[j]; // если j-ый вложен в i-й то говорим шо глубина i-го на единицу больше, т.к чем больше значение в depth, тем дальше от начала отрезок в depth
                if (deep_of_deepest < depth[i]) { // если степень вложенности больше чем предыдущий максимум - обновляем
                    id_of_deepest = i;
                    deep_of_deepest = depth[i];
                }
            }
        }
    }
    int j = 0;
    while (1) {
        answer[j] = segments[id_of_deepest].id; // закидываем в ответ самый вложенный сегмент
        id_of_deepest = parent[id_of_deepest]; // делаем самым вложенным родителя предыдущего(т.е раскручиваем дерево вложенности вверх)
        j++; // идём дальше по сегментам к бОльшим по длине
        if (id_of_deepest == 0) // если мы раскрутили все вложенные отрезки то останавливаем while
            break;
    }
    cout << deep_of_deepest << endl; // выводим степень вложенности
    for (int i = j - 1; i >= 0; i--)
        cout << answer[i] << " "; // выводим отрезки от меньшего к большему
    return 0;
}

bool compare_by_lenght(const segment x, const segment y) {
    return ((x.right - x.left) < (y.right - y.left));
}