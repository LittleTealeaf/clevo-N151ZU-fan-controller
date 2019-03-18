# Clevo N151ZU fan control

Default (bios) fan behavior for this laptop is not optimized, this project brings a smarter automatic fan control. It makes it perfectly silent for less-demanding tasks like office, web, or even video-watch, but gradually power the fan when necessary.

## Getting Started

These instructions will help you to get the software and setup your system to use it.

### Prerequisites

```
g++ build-essential
```

### Installing

Clone the repo:
```
git clone ...
```

Compile with g++:
```
g++ .../ -o .../N151ZU_fan_controler
```

Make executable:
```
chmod +x .../N151ZU_fan_controller
```

Copy to the systemd scripts folder:
```
sudo cp .../clevo-fan.service /etc/systemd/system/
sudo systemctl enable clevo-fan.service
```


End with an example of getting some data out of the system or using it for a little demo

## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
* [Maven](https://maven.apache.org/) - Dependency Management
* [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

* **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone whose code was used
* Inspiration
* etc

 
