<<<<<<< Updated upstream
# IoCContainer

![dLV1Sjis4BtpAtQcCmsbI6hF6bSJsubRTvg94xMJGwVZWSWLYHO2661fHsrpxGVq4_ib1PQKI0fg5Si7cLmi7XPl7nwa5uw4fNgZ4lURr9MmOWEP4ivT4bcvgWblCwENydU4htifIcgaROLWB9KcIPvzyvb_oJDuAKZ0EnGvMWT2v_wva8xizWaeKXFQjSYGqHem-2C107WErh-c-Uf](https://github.com/user-attachments/assets/3d3a9e30-c5b0-4ba1-b03b-dccc0b822be4)

<details>
  <summary>
    PlantUML
  </summary>
   <pre><code class="language-plantuml"> 
  @startuml
  skinparam classAttributeIconSize 0
  skinparam linetype ortho

  '========================================
  ' Data Readers and Registry
  '========================================
interface DataReader {
    + read(dbPath: QString): QList<QStringList>*
}
class CsvReader
class JsonReader
class SqliteReader

DataReader <|.. CsvReader : implements
DataReader <|.. JsonReader : implements
DataReader <|.. SqliteReader : implements

class DataReaderRegistry {
    + instance(): DataReaderRegistry&
    + registerReader(ext: QString, FactoryFn)
    + createReader(ext: QString): std::shared_ptr<DataReader>
}
DataReaderRegistry o-- DataReader : holds factories

note right of DataReaderRegistry
  Functional: Manages registration and
  instantiation of DataReader by file extension.
end note

'========================================
' Graph Widgets and Display Area
'========================================
interface IGraphWidget {
    + setData(data: QList<QStringList>)
    + setStyle(style: IChartStyle*)
}
class CustomPlotGraph
class BarChartGraph
class PieChartGraph
class ScatterGraph

IGraphWidget <|.. CustomPlotGraph : implements
IGraphWidget <|.. BarChartGraph : implements
IGraphWidget <|.. PieChartPieChartWidget : implements
IGraphWidget <|.. ScatterGraph : implements

class GraphDisplayArea {
    - graph_: std::unique_ptr<IGraphWidget>
    + setGraph(graph: std::unique_ptr<IGraphWidget>)
    + setData(data: QList<QStringList>)
}
GraphDisplayArea --> IGraphWidget : uses for rendering

note right of GraphDisplayArea
  Functional: Hosts the graph widget instance
  and delegates data/style to it.
end note

class GraphTypeManager {
    - display_: GraphDisplayArea*
    - factories_: std::vector<function<unique_ptr<IGraphWidget>(QWidget*)>>
    + setType(name: QString)
}

GraphTypeManager --> GraphDisplayArea : updates display
GraphTypeManager ..> IGraphWidget : creates via factories

note left of GraphTypeManager
  Functional: Chooses graph type based on
  user selection and instructs GraphDisplayArea.
end note

'========================================
' Chart Styles and Strategy
'========================================
interface IChartStyle
class ColourChartStyle
class GrayScaleChartStyle

IChartStyle <|.. ColourChartStyle : implements color style
IChartStyle <|.. GrayScaleChartStyle : implements grayscale style

class StyleManager {
    + StyleManager(actColor: QAction*, actGray: QAction*, display: GraphDisplayArea*, parent: QObject*)
}
StyleManager --> IChartStyle : applies style
StyleManager --> GraphDisplayArea : triggers re-render

note right of StyleManager
  Functional: Listens to UI actions to switch
  between color and grayscale styles.
end note

'========================================
' Main Application and Signal-Slot
'========================================
class MainWindow {
    - dataReader: DataReader*
    - displayArea: GraphDisplayArea*
    - fileModel: QFileSystemModel*
    - currentRows_: std::unique_ptr<QList<QStringList>>
    - typeManager_: std::unique_ptr<GraphTypeManager>
    + onBatchExport()
}
MainWindow --> DataReader : loads data
MainWindow --> GraphDisplayArea : provides data
MainWindow --> GraphTypeManager : instructs type change
MainWindow --> QFileSystemModel : manages file browsing

note top of MainWindow
  Functional: Orchestrates UI, data loading,
  graph type selection, and export.
end note

'========================================
' QObject connect relationships (dashed blue arrows)
'========================================
MainWindow ..[#Blue,dashed]-> GraphTypeManager : signal fileSelected() → slot setType()
MainWindow ..[#Blue,dashed]-> StyleManager : signal colorAction.triggered() → slot applyColor()
MainWindow ..[#Blue,dashed]-> StyleManager : signal grayAction.triggered() → slot applyGray()
MainWindow ..[#Blue,dashed]-> DataReaderRegistry : signal fileExtensionSelected() → slot createReader()
  @enduml
</code></pre>
</details>

# Обзор проекта

IoCКонтейнер — простое приложение на Qt, демонстрирующее использование контейнера инверсии управления (IoC) для разделения:
- загрузки данных,
- отрисовки графиков,
- применения стилей,
- экспорта в PDF, PNG, JPEG.

Поддерживаются форматы CSV, JSON и SQLite, а также несколько типов графиков (столбчатый, круговая диаграмма, точечный, CustomPlot) с возможностью переключения между цветным и чёрно-белым стилями.

# Архитектура

Приложение построено по модульному и многоуровневому принципу:

1. **Чтение данных** (`DataReader`, `CsvReader`, `JsonReader`, `SqliteReader`, `DataReaderRegistry`):
   - Паттерн «Стратегия» для подключаемых источников данных.
   - Реестр управляет функциями-фабриками для создания читателей по расширению файла.

2. **Виджеты графиков** (`IGraphWidget`, конкретные графики, `GraphDisplayArea`, `GraphTypeManager`):
   - Паттерн «Абстрактная фабрика» для создания виджетов графиков.
   - `GraphDisplayArea` содержит выбранный виджет и делегирует ему отрисовку.

3. **Стили графиков** (`IChartStyle`, `ColourChartStyle`, `GrayScaleChartStyle`, `StyleManager`):
   - Паттерн «Стратегия» для применения стилей в рантайме.
   - Менеджер стилей слушает сигналы Qt для переключения между стилями.

4. **Контейнер IoC** (`MyContainer`, `ServiceInterface`, `ServiceImplementation`):
   - Упрощённый контейнер для регистрации и получения зависимостей.
   - Обеспечивает слабую связанность и упрощает тестирование.

5. **Главное окно** (`MainWindow`):
   - Координирует работу UI, загрузку данных, выбор графика, стили и экспорт в PDF.
   - Использует механизм сигналов/слотов Qt для развязки компонентов.

# Архитектурные решения и паттерны

- **IoC-контейнер**: централизует управление зависимостями, уменьшает «жесткую» привязку в `main.cpp`, улучшает тестируемость.
- **Стратегия**: для читателей данных и стилей, что позволяет легко расширять функционал.
- **Абстрактная фабрика**: `GraphTypeManager` создаёт нужный граф без раскрытия деталей реализации UI.
- **Синглтон/реестр**: `DataReaderRegistry` — единственная точка регистрации и создания `DataReader`.
- **Сигналы/слоты Qt**: разграничивают события UI и бизнес-логику, обеспечивая гибкую конфигурацию взаимодействий.

# Преимущества

- **Высокая расширяемость**: легко добавить новый источник данных, тип графика или стиль.
- **Слабая связанность**: модули зависят от абстракций, что упрощает сопровождение и тестирование.
- **Чёткое разделение ответственности**: каждый компонент отвечает за свою задачу.

# Недостатки

- **Сложность при старте**: ввод IoC и нескольких паттернов может быть избыточен для небольшой лабораторной.
- **Небольшие накладные расходы**: динамическое создание через фабрики и виртуальные вызовы.
- **Кривая обучения**: нужно понять несколько паттернов и модель событий Qt.

# Инструкции по сборке и запуску

```bash
# Клонировать и перейти на ветку dev
git clone https://github.com/Helbushek/IocContainer.git
cd IocContainer
git checkout dev

# Собрать с помощью qmake
qmake IocContainer.pro && make

# Запустить
./IocContainer
```

## Автор

Горевой Александр, для лабораторной работы №3 по курсу TRPO.
=======
# IocContainer

<!-- UML Diagram Placeholder: insert rendered PlantUML here -->

# Project Overview

IocContainer is a simple Qt-based application demonstrating the use of an Inversion of Control (IoC) container for decoupling data loading, graph rendering, styling, and PDF export. It supports multiple data formats (CSV/JSON/SQLite) and graph types (bar, pie, scatter, custom plots) with interchangeable color and grayscale styles.

# Architecture

The application follows a modular, layered architecture:

1. **Data Readers** (DataReader, CsvReader, JsonReader, SqliteReader, DataReaderRegistry):
 - Implements the Strategy pattern for pluggable data sources.
 - Registry manages factory functions to instantiate readers based on file extension.

2. **Graph Widgets** (IGraphWidget, concrete graphs, GraphDisplayArea, GraphTypeManager):
 - Abstract factory for graph creation.
 - Display area hosts the selected graph widget and delegates rendering.

3. **Chart Styles** (IChartStyle, ColourChartStyle, GrayScaleChartStyle, StyleManager):
 - Strategy pattern to apply styling (color vs grayscale) at runtime.
 - Manager listens to UI actions (via Qt signals/slots) to switch styles.

4. **IoC Container** (MyContainer, ServiceInterface, ServiceImplementation):
 - Simplified generic container for registering and resolving dependencies.
 - Enables loose coupling and easier testing.

5. **Main Window** (MainWindow):
 - Orchestrates UI, data loading, graph selection, styling, and PDF export.
 - Uses Qt’s signal/slot mechanism to wire components without tight coupling.

## UML Diagram

## Architectural Decisions & Patterns

- **Inversion of Control (IoC) Container**: Centralizes dependency management, reduces manual wiring in main.cpp, and improves testability.
- **Strategy Pattern**: Used for both data readers and chart styles to allow easy extension (e.g., new file format or style).
- **Abstract Factory**: GraphTypeManagercreates appropriateIGraphWidgetwithout exposing implementation details to the UI.
- **Singleton/Registry**:DataReaderRegistry` ensures a single point for registering and creating readers by file extension.
- Qt Signal/Slot: Decouples UI events from business logic, enabling runtime reconfiguration of graph type and style.

## Advantages

- High Extensibility: New readers, graphs, or styles can be added by implementing respective interfaces and registering them.
- Loose Coupling: Components depend on abstractions, simplifying maintenance and unit testing.
- Clear Separation of Concerns: Each module focuses on a single responsibility (data, rendering, styling, orchestration).

## Disadvantages

- Initial Complexity: Introducing an IoC container and multiple patterns can be overkill for small labs.
- Performance Overhead: Factory lookups and dynamic dispatch add negligible but non-zero runtime cost.
- Learning Curve: Students must understand several design patterns and Qt’s event model.

# Build & Run Instructions

bash
## Clone
```
git clone https://github.com/Helbushek/IocContainer.git
cd IocContainer
```
## Build with qmake
```
qmake IocContainer.pro && make
```
# Run
```
./IocContainer
```
## Contributing

Feel free to open issues or pull requests for bug fixes, new readers, graph types, or style extensions.

## Author

Gorevoy Aleksandr, 
>>>>>>> Stashed changes
