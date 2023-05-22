# final-project

## Abstract
SDR (Software Defined Radio) provides flexible, reproducible and longer-lasting radio tools for military and civilian wireless communications infrastructure. SDR is a radio (radio frequency) communication system whose components are implemented as software. In this study, it is aimed to establish multi-channel wireless communication with FANET between two SDRs, to send location information as data, to examine the effects of different modulation techniques and environmental factors on data transport. It is aimed to use multi-channel token distribution as link layer protocol, MIMO as communication access technology and GNU Radio platform for SDR software development. The structures of the communication layer such as the protocols, communication systems, network structures suggested in the studies in the literature are generally tested in the simulation environment. The simulation environment provides researchers with fast and easy development and testing, but there are also disadvantages. These cause a product to be isolated from hardware, software and cost effects that may be encountered while developing, and from environmental factors affecting the communication channel while testing. In this study, it is aimed to establish a realistic test environment, and another contribution of the study is to present the developed block diagrams and codes as clear and reproducible.

## GNU Radio Block Diagram
![](screenshots/system_design.png)


## Citation

```
@inproceedings{bloessl2013ieee,
  title={An IEEE 802.11 a/g/p OFDM Receiver for GNU Radio},
  author={Bloessl, Bastian and Segata, Michele and Sommer, Christoph and Dressler, Falko},
  booktitle={Proceedings of the second workshop on Software radio implementation forum},
  pages={9--16},
  year={2013}
}
```
```
@github{github.com/zaeemzadeh/MIMO-OFDM-Alamouti,
  title={MIMO-OFDM with Alamouti space-time coding},
  author={Zaeemzadeh, Alireza},
  year={2014}
}
```
```
@github{github.com/kit-cel/gnuradio, github.com/MLsmd/GSoC18-proposal
  title={MIMO basics for GNU Radio},
  author={Schmid, Moritz Luca},
  year={2018}
}
```
